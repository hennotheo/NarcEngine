//
// Created by theohenno on 11/27/25.
//

#pragma once

namespace narc_engine {
    class VulkanSwapChain;
    class VulkanPipelineLayout;
    class VulkanRenderPass;
    class VulkanDevice;

    class VulkanGraphicsPipeline final : public IGraphicsPipeline
    {
    public:
        explicit VulkanGraphicsPipeline(const VulkanDevice* device, const VulkanSwapChain* swapChain, const VulkanPipelineLayout* pipelineLayout, std::unique_ptr<VulkanRenderPass>& renderPass);
        ~VulkanGraphicsPipeline() override;

        NARC_IMPL_INITIALISABLE();

        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(const VertexLayout& value);

        NARC_GETTER(VulkanRenderPass*, getRenderPass, m_renderPass.get());
        NARC_GETTER(VkPipeline, getHandle, m_pipeline);
        NARC_GETTER(const VulkanPipelineLayout*, getLayout, m_pipelineLayout);
        
    private:
        const VulkanDevice* m_device;
        const VulkanSwapChain* m_swapChain;
        const VulkanPipelineLayout* m_pipelineLayout;

        std::unique_ptr<VulkanRenderPass> m_renderPass;

        VkPipeline m_pipeline = VK_NULL_HANDLE;
    };
} // narc_engine
