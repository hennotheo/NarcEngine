//
// Created by theoh on 3/16/2025.
//

#pragma once
#include <vulkan/vulkan_core.h>

namespace narc_engine {
    class CommandBuffer;
    class Shader;
    class RenderPass;

    class GraphicsPipeline
    {
    public:
        explicit GraphicsPipeline(const RenderPass* renderPass, const Shader* shader);
        ~GraphicsPipeline();

        VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();
        VkPipelineViewportStateCreateInfo createViewportStateInfo();
        VkPipelineDepthStencilStateCreateInfo createDepthStencilStateInfo();
        VkPipelineRasterizationStateCreateInfo createRasterizationStateInfo();
        VkPipelineMultisampleStateCreateInfo createMultisampleStateInfo();
        VkPipelineColorBlendAttachmentState createColorBlendAttachmentState();
        VkPipelineColorBlendStateCreateInfo createColorBlendStateInfo(const VkPipelineColorBlendAttachmentState& colorBlendAttachment);
        VkPipelineDynamicStateCreateInfo createDynamicStateInfo(const std::vector<VkDynamicState>& dynamicStates);

        GETTER VkPipelineLayout getLayout() const { return m_pipelineLayout; }

        void bindPipeline(const CommandBuffer* commandBuffer) const;

    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        VkPipelineVertexInputStateCreateInfo createPipelineVertexInputInfo(VkVertexInputBindingDescription vertexDescriptions,
                                                                           const std::array<VkVertexInputAttributeDescription, 3>& attributeDescription);
    };
}
