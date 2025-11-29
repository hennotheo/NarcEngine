//
// Created by theohenno on 11/28/25.
//

#include "vulkan_wrappers/VulkanPipelineLayout.h"

#include "vulkan_wrappers/VulkanDevice.h"

namespace narc_engine {
    VulkanPipelineLayout::VulkanPipelineLayout(const std::weak_ptr<VulkanDevice>& device) :
        m_device(device)
    {
    }

    VulkanPipelineLayout::~VulkanPipelineLayout()
    {
    }

    void VulkanPipelineLayout::init()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device.");

        if (vkCreatePipelineLayout(device->getHandle(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create pipeline layout!");
        }
    }

    void VulkanPipelineLayout::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device.");

        vkDestroyPipelineLayout(device->getHandle(), m_pipelineLayout, nullptr);
    }
} // narc_engine
