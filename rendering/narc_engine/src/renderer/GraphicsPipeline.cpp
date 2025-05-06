//
// Created by theoh on 3/16/2025.
//

#include "renderer/GraphicsPipeline.h"

#include "CommandBuffer.h"
#include "models/Vertex.h"
#include "models/PushConstants.h"

#include "renderer/RenderPass.h"

#include "models/Shader.h"

namespace narc_engine {
    GraphicsPipeline::GraphicsPipeline(const RenderPass* renderPass,
        const Shader* vertShader, const Shader* fragShader)
        : DeviceComponent()
    {

        std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages = {
            vertShader->configureShaderStage("main", VK_SHADER_STAGE_VERTEX_BIT),
            fragShader->configureShaderStage("main", VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        const auto vertexDescriptions = Vertex::getBindingDescription();
        const auto attributeDescription = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = createPipelineVertexInputInfo(vertexDescriptions, attributeDescription);
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = createInputAssemblyStateInfo();
        VkPipelineViewportStateCreateInfo viewportState = createViewportStateInfo();
        VkPipelineDepthStencilStateCreateInfo depthStencil = createDepthStencilStateInfo();
        VkPipelineRasterizationStateCreateInfo rasterizer = createRasterizationStateInfo();
        VkPipelineMultisampleStateCreateInfo multisampling = createMultisampleStateInfo();

        VkPipelineColorBlendAttachmentState colorBlendAttachment = createColorBlendAttachmentState();
        VkPipelineColorBlendStateCreateInfo colorBlending = createColorBlendStateInfo(colorBlendAttachment);

        const std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState = createDynamicStateInfo(dynamicStates);

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstants);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts =
        {
            vertShader->getDescriptorSetLayout(),
            fragShader->getDescriptorSetLayout()
        };
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(getVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = m_shaderStages.data();

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = renderPass->getRenderPass();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        if (vkCreateGraphicsPipelines(getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create graphics pipeline!");
        }
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        vkDestroyPipeline(getVkDevice(), m_pipeline, nullptr);
        vkDestroyPipelineLayout(getVkDevice(), m_pipelineLayout, nullptr);
    }

    VkPipelineVertexInputStateCreateInfo GraphicsPipeline::createPipelineVertexInputInfo(const VkVertexInputBindingDescription vertexDescriptions,
        const std::array<VkVertexInputAttributeDescription, 3>& attributeDescription)
    {
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(Vertex::getAttributeDescriptions().size());
        vertexInputInfo.pVertexBindingDescriptions = &vertexDescriptions; // Optional
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data(); // Optional

        return vertexInputInfo;
    }

    VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::createInputAssemblyStateInfo()
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        return inputAssembly;
    }

    VkPipelineViewportStateCreateInfo GraphicsPipeline::createViewportStateInfo()
    {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        return viewportState;
    }

    VkPipelineDepthStencilStateCreateInfo GraphicsPipeline::createDepthStencilStateInfo()
    {
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        return depthStencil;
    }

    VkPipelineRasterizationStateCreateInfo GraphicsPipeline::createRasterizationStateInfo()
    {
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        return rasterizer;
    }

    VkPipelineMultisampleStateCreateInfo GraphicsPipeline::createMultisampleStateInfo()
    {
        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        return multisampling;
    }

    VkPipelineColorBlendAttachmentState GraphicsPipeline::createColorBlendAttachmentState()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        return colorBlendAttachment;
    }

    VkPipelineColorBlendStateCreateInfo GraphicsPipeline::createColorBlendStateInfo(const VkPipelineColorBlendAttachmentState& colorBlendAttachment)
    {
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        return colorBlending;
    }

    VkPipelineDynamicStateCreateInfo GraphicsPipeline::createDynamicStateInfo(const std::vector<VkDynamicState>& dynamicStates)
    {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        return dynamicState;
    }

    void GraphicsPipeline::bindPipeline(const CommandBuffer* commandBuffer) const
    {
        commandBuffer->cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    }
} // narc_engine
