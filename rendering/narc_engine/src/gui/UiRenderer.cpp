#include "gui/UiRenderer.h"

#include "Engine.h"
#include "core/EngineInstance.h"
#include "renderer/MultiFrameManager.h"
#include "renderer/SwapChain.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace narc_engine
{
    UiRenderer::UiRenderer(const EngineInstance *instance, const MultiFrameManager *frameManager, const SwapChain *swapChain)
    {
        Engine *engine = Engine::getInstance();
        const DeviceHandler *device = engine->getDevice();

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = instance->getvkInstance();
        device->setupImGui(&initInfo);
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

        // ImGui_ImplVulkan_Init(&initInfo);
    }

    UiRenderer::~UiRenderer()
    {
        // ImGui_ImplVulkan_Shutdown();
        // ImGui_ImplGlfw_Shutdown();
        // ImGui::DestroyContext();
    }
}