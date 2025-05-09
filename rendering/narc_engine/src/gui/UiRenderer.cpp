#include "gui/UiRenderer.h"

#include "Engine.h"
#include "core/EngineInstance.h"
#include "CommandBuffer.h"
#include "renderer/MultiFrameManager.h"
#include "renderer/SwapChain.h"
#include "core/interfaces/ISurfaceProvider.h"
#include "interfaces/IGuiHandle.h"
#include "gui/GuiWindow.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace narc_gui
{
    UiRenderer::UiRenderer(const narc_engine::EngineInstance* instance, const narc_engine::MultiFrameManager* frameManager, const narc_engine::SwapChain* swapChain, const narc_engine::ISurfaceProvider* surface)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        narc_engine::Engine* engine = narc_engine::Engine::getInstance();
        const narc_engine::DeviceHandler* device = engine->getDevice();

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = instance->get();
        initInfo.PhysicalDevice = device->getPhysicalDevice()->getVkPhysicalDevice();
        initInfo.Device = device->getLogicalDevice()->get();
        initInfo.QueueFamily = device->getPhysicalDevice()->getQueueFamilyIndices().GraphicsFamily.value();
        initInfo.Queue = engine->getGraphicsQueue()->getVkQueue();
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = frameManager->getMaxFrameInFlight();
        initInfo.DescriptorPool = frameManager->getDescriptorPool()->get();
        initInfo.RenderPass = swapChain->getRenderPass()->getRenderPass();
        initInfo.CheckVkResultFn = [](VkResult err)
            {
                if (err != VK_SUCCESS)
                {
                    NARCLOG_FATAL(std::string("Vulkan Error: %d", err));
                }
            };

        ImGui_ImplVulkan_Init(&initInfo);
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)surface->getSurfaceHandler(), true);
    }

    UiRenderer::~UiRenderer()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UiRenderer::beginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UiRenderer::render(const narc_engine::CommandBuffer* commandBuffer)
    {
        for (const auto& component : m_guiComponents)
        {
            component->render();
        }

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer->getVkCommandBuffer());
    }
}