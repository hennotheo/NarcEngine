//
// Created by theohenno on 11/27/25.
//

#pragma once

namespace narc_engine {
    class VulkanSwapChain;
    class VulkanPipelineLayout;
    class VulkanRenderPass;
    class VulkanDevice;

    class VulkanGraphicsPipeline final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanGraphicsPipeline(const std::weak_ptr<VulkanDevice>& device, const std::unique_ptr<VulkanSwapChain>& swapChain);
        ~VulkanGraphicsPipeline() override;

        NARC_IMPL_INITIALISABLE();

        void setLayout(std::unique_ptr<VulkanPipelineLayout>& pipelineLayout);
        void setRenderPass(std::unique_ptr<VulkanRenderPass>& renderPass);

    private:
        std::weak_ptr<VulkanDevice> m_device;
        VulkanSwapChain* m_swapChain;
        
        std::unique_ptr<VulkanRenderPass> m_renderPass;
        std::unique_ptr<VulkanPipelineLayout> m_pipelineLayout;

        VkPipeline m_pipeline = VK_NULL_HANDLE;
    };
} // narc_engine
