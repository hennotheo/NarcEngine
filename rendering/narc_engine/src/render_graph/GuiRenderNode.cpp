#include "render_graph/GuiRenderNode.h"

#include <imgui.h>

#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

#include "Engine.h"
#include "platform/vulkan/SwapChain.h"
#include "renderer/MultiFrameManager.h"

namespace narc_engine
{
    GuiRenderNode::GuiRenderNode(const RenderPass* renderPass, const SwapChain* swapchain,
        const MultiFrameManager* frameManager, const Window* window)
        : RenderNode()
    {
#pragma warning "Add better order management"
        m_order = 999;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        narc_engine::Engine* engine = Engine::getInstance();
        const narc_engine::DeviceHandler* device = engine->getDevice();

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = engine->getEngineInstance()->get();
        initInfo.PhysicalDevice = NARC_PHYSICAL_DEVICE_HANDLE;
        initInfo.Device = NARC_DEVICE_HANDLE;
        initInfo.QueueFamily = device->getPhysicalDevice()->getQueueFamilyIndices().GraphicsFamily.value();
        initInfo.Queue = engine->getGraphicsQueue()->getVkQueue();
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = frameManager->getMaxFrameInFlight();
        initInfo.DescriptorPool = frameManager->getDescriptorPool()->get();
        initInfo.RenderPass = swapchain->getRenderPass()->get();
        initInfo.CheckVkResultFn = [](VkResult err)
            {
                if (err != VK_SUCCESS)
                {
                    NARCLOG_FATAL(std::string("Vulkan Error: %d", err));
                }
            };

        ImGui_ImplVulkan_Init(&initInfo);
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window->getSurfaceHandler(), true);
    }

    GuiRenderNode::~GuiRenderNode()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiRenderNode::record(const CommandBuffer* cmd, const RenderContext* ctx)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd->getVkCommandBuffer());
    }
} // namespace narc_engine
