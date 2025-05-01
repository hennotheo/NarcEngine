#include "gui/UiRenderer.h"

#include "Engine.h"
#include "core/EngineInstance.h"
#include "CommandBuffer.h"
#include "renderer/MultiFrameManager.h"
#include "renderer/SwapChain.h"
#include "core/interfaces/ISurfaceProvider.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace narc_engine
{
    UiRenderer::UiRenderer(const EngineInstance* instance, const MultiFrameManager* frameManager, const SwapChain* swapChain, const ISurfaceProvider* surface)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        Engine* engine = Engine::getInstance();
        const DeviceHandler* device = engine->getDevice();

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = instance->getvkInstance();
        initInfo.PhysicalDevice = device->getPhysicalDevice()->getVkPhysicalDevice();
        initInfo.Device = device->getLogicalDevice()->getVkDevice();
        initInfo.QueueFamily = device->getPhysicalDevice()->getQueueFamilyIndices().GraphicsFamily.value();
        initInfo.Queue = engine->getGraphicsQueue()->getVkQueue();
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = frameManager->getMaxFrameInFlight();
        initInfo.DescriptorPool = frameManager->getDescriptorPool()->getVkDescriptorPool();
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

    void UiRenderer::render(const CommandBuffer* commandBuffer)
    {
        ImGui::Begin("Hello, Vulkan!");
        ImGui::Text("Ceci est une interface ImGui avec Vulkan.");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer->getVkCommandBuffer());
    }
}