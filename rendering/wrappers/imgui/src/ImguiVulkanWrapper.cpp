//
// Created by Theo on 5/9/2026.
//

#include "ImguiVulkanWrapper.h"

#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace narc_engine {

    ImguiVulkanWrapper::ImguiVulkanWrapper(const NarcImGuiContext& context) : m_vulkanContext(context)
    {

    }

    ImguiVulkanWrapper::~ImguiVulkanWrapper() = default;

    void ImguiVulkanWrapper::init()
    {
        IMGUI_CHECKVERSION();
        m_context = ImGui::CreateContext();

        void* windowHandle = m_vulkanContext.window->getHandle();
        auto t = static_cast<GLFWwindow*>(windowHandle);
        ImGui_ImplGlfw_InitForVulkan(t, true);
        // ImGui_ImplVulkan_Init();

        const VulkanGraphicsInstance* vulkanInstance =  narc_core::backend_cast<VulkanGraphicsInstance, IGraphicsInstance>(m_vulkanContext.GraphicsInstance);
        const auto* graphicsQueue = narc_core::backend_cast<VulkanQueue, IQueue>(vulkanInstance->getGraphicsQueue());
        const auto* pass = narc_core::backend_cast<VulkanGraphicsPipeline, IGraphicsPipeline>(m_vulkanContext.Pipeline);

        createDescriptorPool();

        ImGui_ImplVulkan_InitInfo initInfo{};
        initInfo.Instance = vulkanInstance->getInstance()->getHandled();
        initInfo.PhysicalDevice = vulkanInstance->getDevice()->getPhysicalDeviceHandle();
        initInfo.Device = vulkanInstance->getDevice()->getHandle();
        initInfo.QueueFamily = graphicsQueue->getQueueFamilyIndex();
        initInfo.Queue = graphicsQueue->getHandle();
        initInfo.DescriptorPool = m_descriptorPool;
        initInfo.MinImageCount = m_vulkanContext.MinImageCount;
        initInfo.ImageCount = m_vulkanContext.ImageCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.RenderPass = pass->getRenderPass()->getHandle();

        ImGui_ImplVulkan_Init(&initInfo);
    }

    void ImguiVulkanWrapper::shutdown()
    {
        const VulkanGraphicsInstance* vulkanInstance =  narc_core::backend_cast<VulkanGraphicsInstance, IGraphicsInstance>(m_vulkanContext.GraphicsInstance);

        vulkanInstance->getDevice()->waitIdle();

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        if (m_descriptorPool != VK_NULL_HANDLE) //TODO: Horrible
        {
            vkDestroyDescriptorPool(vulkanInstance->getDevice()->getHandle(), m_descriptorPool, nullptr);
            m_descriptorPool = VK_NULL_HANDLE;
        }

        ImGui::DestroyContext(m_context);
        m_context = nullptr;
    }

    void ImguiVulkanWrapper::newFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImguiVulkanWrapper::endFrame()
    {
        ImGui::EndFrame();
    }

    void ImguiVulkanWrapper::render()
    {
        ImGui::Render();
    }

    void ImguiVulkanWrapper::createDescriptorPool()
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
        poolInfo.pPoolSizes = poolSizes;

        const VulkanGraphicsInstance* vulkanInstance =  narc_core::backend_cast<VulkanGraphicsInstance, IGraphicsInstance>(m_vulkanContext.GraphicsInstance);

        if (vkCreateDescriptorPool(
                vulkanInstance->getDevice()->getHandle(),
                &poolInfo,
                nullptr,
                &m_descriptorPool
            ) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create ImGui descriptor pool.");
        }
    }
}
