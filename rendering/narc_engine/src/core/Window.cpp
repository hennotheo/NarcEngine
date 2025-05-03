#include "core/Window.h"

#include <NarcLog.h>
#include <NarcCore.h>

#include "core/EngineInstance.h"
#include "core/EngineBuilder.h"
#include "core/devices/DeviceHandler.h"

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

        if (glfwCreateWindowSurface(m_engineInstance->getvkInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create window surface!");
        }
    }

    Window::~Window()
    {
        //Destroy before the window is destroyed
        m_renderer.reset();
        m_frameManager.reset();

        vkDestroySurfaceKHR(m_engineInstance->getvkInstance(), m_surface, nullptr);

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::initRenderingSystem(const EngineBuilder* builder)
    {
        m_physicalDevice = builder->getPhysicalDevice();
        m_logicalDevice = builder->getLogicalDevice();
        
        m_frameManager = std::make_unique<MultiFrameManager>(builder->getFrameInFlightCount());
        m_renderer = std::make_unique<EngineRenderer>(m_engineInstance, this, m_frameManager.get());
    }

    void Window::render()
    {
        const FrameHandler* frameHandler = m_frameManager->getCurrentFrameHandler();
        VkDevice device = m_logicalDevice->getVkDevice();

        const std::vector<VkFence> inFlightFencesToWait = { frameHandler->getInFlightFence() };
        vkWaitForFences(device, 1, inFlightFencesToWait.data(), VK_TRUE, UINT64_MAX);

        m_renderer->prepareFrame(frameHandler);

        vkResetFences(device, 1, inFlightFencesToWait.data());

        SignalSemaphores signalSemaphores = m_renderer->drawFrame(frameHandler);
        m_renderer->presentFrame(signalSemaphores);

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

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->notifyFramebufferResized(width, height);
    }

    void Window::onKeyboardInputPerformed(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_onKeyboardEvent.trigger(key, scancode, action, mods);
    }

    void Window::onMouseInputPerformed(GLFWwindow* window, int button, int action, int mods)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_onMouseEvent.trigger(button, action, mods);
    }
}