//
// Created by theohenno on 11/27/25.
//

#include "pipeline/VulkanGraphicsPipeline.h"

#include "device/VulkanDevice.h"
#include "pipeline/VulkanPipelineLayout.h"
#include "pipeline/VulkanRenderPass.h"
#include "VulkanShaderModule.h"
#include "mapping/mappingToVk.h"

namespace narc_engine {
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(const VulkanDevice* device, const VulkanSwapChain* swapChain,
                                                   const VulkanPipelineLayout* pipelineLayout, std::unique_ptr<VulkanRenderPass>& renderPass) :
        m_device(device),
        m_swapChain(swapChain),
        m_pipelineLayout(pipelineLayout),
        m_renderPass(std::move(renderPass))
    {
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline() = default;

    void VulkanGraphicsPipeline::init()
    {
        NARC_GUARD_RAW_PTR(m_pipelineLayout, "PipelineLayout not set for VulkanGraphicsPipeline.");
        NARC_GUARD_RAW_PTR(m_renderPass, "RenderPass not set for VulkanGraphicsPipeline.");

        m_renderPass->init(); //TODO: Out of here, must have cache of passess

        auto fragShaderModule = m_pipelineLayout->createFragmentShaderModule();
        fragShaderModule.init();
        auto vertShaderModule = m_pipelineLayout->createVertexShaderModule();
        vertShaderModule.init();

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule.getHandle();
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule.getHandle();
        fragShaderStageInfo.pName = "main";

        std::array shaderStages = {vertShaderStageInfo, fragShaderStageInfo};

        //Dynamic States
        std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();


        //Vert input
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        const auto& vertexLayout = m_pipelineLayout->getVertexLayout();

        auto bindingDescription = mapping::mapFromVertexLayout(vertexLayout);
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

        auto attributeDescriptions = getAttributeDescriptions(vertexLayout);

        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        //Input Assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        NARC_GUARD_RAW_PTR(m_swapChain, "SwapChain not set for VulkanGraphicsPipeline.");

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

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

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                              VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

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

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_pipelineLayout->getHandle();
        pipelineInfo.renderPass = m_renderPass->getHandle();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");
        if (vkCreateGraphicsPipelines(m_device->getHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create graphics pipeline!");
        }

        vertShaderModule.shutdown();
        fragShaderModule.shutdown();
    }

    void VulkanGraphicsPipeline::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");
        vkDestroyPipeline(m_device->getHandle(), m_pipeline, nullptr);

        m_renderPass->shutdown();
    }

    std::vector<VkVertexInputAttributeDescription> VulkanGraphicsPipeline::getAttributeDescriptions(const VertexLayout& value)
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        for (auto attribute: value.Attributes)
        {
            attributeDescriptions.push_back(mapping::mapFromVertexAttribute(attribute));
        }

        return attributeDescriptions;
    }
} // narc_engine
