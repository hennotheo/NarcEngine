#include "core/Window.h"

#include "Engine.h"

#include "core/EngineInstance.h"
#include "core/EngineBuilder.h"
#include "core/devices/DeviceHandler.h"
#include "models/Shader.h"
#include "renderer/render_graph/RenderNode.h"

namespace narc_engine
{
    constexpr uint32_t g_width = 800;
    constexpr uint32_t g_height = 600;

    Window::Window(const EngineInstance* engineInstance)
    {
        m_engineInstance = engineInstance;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(g_width, g_height, "Narc Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
        glfwSetKeyCallback(m_window, onKeyboardInputPerformed);
        glfwSetMouseButtonCallback(m_window, onMouseInputPerformed);

        if (glfwCreateWindowSurface(m_engineInstance->get(), m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create window surface!");
        }
    }

    Window::~Window()
    {
        //Destroy before the window is destroyed
        m_swapchain->cleanSwapChain();

        m_renderGraph.reset();

        m_swapchain->cleanRenderPass();

        m_frameManager.reset();

        vkDestroySurfaceKHR(m_engineInstance->get(), m_surface, nullptr);

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::initRenderingSystem(const EngineBuilder* builder)
    {
        m_physicalDevice = builder->getPhysicalDevice();
        m_logicalDevice = builder->getLogicalDevice();

        m_frameManager = std::make_unique<MultiFrameManager>(builder->getFrameInFlightCount());

        m_swapchain = std::make_unique<SwapChain>();
        m_swapchain->create(this);
        m_swapchain->createFramebuffers();

        m_renderGraph = std::make_unique<RenderGraph>(m_swapchain.get());
    }

    void Window::render()
    {
        const FrameHandler* frameHandler = m_frameManager->getCurrentFrameHandler();

        const std::vector<VkFence> inFlightFencesToWait = { frameHandler->getInFlightFence()->get() };
        vkWaitForFences(m_logicalDevice->get(), 1, inFlightFencesToWait.data(), VK_TRUE, UINT64_MAX);
        vkResetFences(m_logicalDevice->get(), 1, inFlightFencesToWait.data());

        uint32_t currentImageIndex = 0;
        m_swapchain->acquireNextImage(frameHandler->getImageAvailableSemaphore(), &currentImageIndex);

        m_renderGraph->buildGraph();
        SignalSemaphores signalSemaphores = m_renderGraph->executeGraph(frameHandler, currentImageIndex);

        present(signalSemaphores, currentImageIndex);

        m_frameManager->nextFrame();
    }

    void Window::pollEvents()
    {
        glfwPollEvents();

        m_shouldClose = glfwWindowShouldClose(m_window);

        glfwGetCursorPos(m_window, &m_mouseXpos, &m_mouseYpos);
        m_time = glfwGetTime();
    }

    const char** Window::getRequiredInstanceExtensions(uint32_t* glfwExtensionCount)
    {
        return glfwGetRequiredInstanceExtensions(glfwExtensionCount);
    }

    void Window::getValidFramebufferSize(int* width, int* height) const
    {
        getFramebufferSize(width, height);
        while (*width == 0 || *height == 0)
        {
            getFramebufferSize(width, height);
            glfwWaitEvents();
        }
    }

    void Window::getFramebufferSize(int* width, int* height) const
    {
        glfwGetFramebufferSize(m_window, width, height);
    }

    void Window::present(const SignalSemaphores& signalSemaphores, uint32_t currentImageIndex)
    {
        const VkSwapchainKHR swapChains[] = { m_swapchain->get() };
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &currentImageIndex;
        presentInfo.pResults = nullptr;

        const VkResult result = Engine::getInstance()->getPresentQueue()->presentKHR(&presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized)
        {
            m_framebufferResized = false;
            m_swapchain->reCreate();
        }
        else if (result != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to present swap chain image!");
        }
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        Window* windowObject = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        windowObject->notifyFramebufferResized(width, height);

        windowObject->m_framebufferResized = true;
    }

    void Window::onKeyboardInputPerformed(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* windowObject = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        windowObject->m_onKeyboardEvent.trigger(key, scancode, action, mods);
    }

    void Window::onMouseInputPerformed(GLFWwindow* window, int button, int action, int mods)
    {
        Window* windowObject = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        windowObject->m_onMouseEvent.trigger(button, action, mods);
    }

#warning "TODO: REMOVE THIS"
    // bool firstTime = true;
    void Window::addRenderer(const Renderer* renderer)
    {
        bool materialAlreadyUsed = std::any_of(m_renderGraph->m_renderers.begin(), m_renderGraph->m_renderers.end(), [&](const Renderer* r) {
            return r->getMaterial() == renderer->getMaterial();
            });

        if (!materialAlreadyUsed)
        {
            if (std::find(m_renderGraph->m_renderers.begin(), m_renderGraph->m_renderers.end(), renderer) == m_renderGraph->m_renderers.end())
            {
                Material* material = reinterpret_cast<Material*>(Engine::getInstance()->resourceManager()->getResource(renderer->getMaterial()));
                // if (firstTime)
                {
                    m_renderGraph->addNode(new RenderNode(m_swapchain->getRenderPass(), material->getVertShader(), material->getFragShader()));
                    // firstTime = false;
                }

                std::vector<VkDescriptorSetLayout> layouts =
                {
                    material->getVertShader()->getDescriptorSetLayout()//TODO ATM ALL LAOYOUTS ARE THE SAME
                    // renderer->getMaterial()->getFragShader()->getDescriptorSetLayout()
                };
                VkDescriptorSetAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorSetCount = layouts.size();
                allocInfo.pSetLayouts = layouts.data();

                std::vector<ResourceId> setIds
                {
                    renderer->getMaterial()
                };

                m_frameManager->allocateDescriptorSets(setIds, allocInfo);
            }
            else
            {
                NARCLOG_DEBUG("Renderer already in the graph");
            }
        }
        else
        {
            NARCLOG_DEBUG("Renderer with the same material already exists in the graph");
        }

        m_renderGraph->m_renderers.push_back(renderer);
    }
}