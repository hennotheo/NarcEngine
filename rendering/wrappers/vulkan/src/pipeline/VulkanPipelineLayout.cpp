//
// Created by theohenno on 11/28/25.
//

#include "pipeline/VulkanPipelineLayout.h"

#include "descriptor/VulkanDescriptorSetLayout.h"
#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice* device) :
        m_device(device)
    {
    }

    VulkanPipelineLayout::~VulkanPipelineLayout() = default;

    void VulkanPipelineLayout::init()
    {
        std::vector<VkDescriptorSetLayout> setLayouts;
        setLayouts.reserve(m_setLayouts.size());
        std::ranges::transform(
                m_setLayouts,
                std::back_inserter(setLayouts),
                [](const VulkanDescriptorSetLayout* layout) {
                    return layout->getHandle();
                });

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = setLayouts.size();
        pipelineLayoutInfo.pSetLayouts = setLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        NARC_GUARD_RAW_PTR(m_device, "Failed to get Device.");

        if (vkCreatePipelineLayout(m_device->getHandle(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create pipeline layout!");
        }

        NARC_LOG_DEBUG("Pipeline Layout Created successfully.");
    }

    void VulkanPipelineLayout::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Device.");

        vkDestroyPipelineLayout(m_device->getHandle(), m_pipelineLayout, nullptr);

        NARC_LOG_DEBUG("Pipeline Layout Destroyed successfully.");
    }

    void VulkanPipelineLayout::addDescriptorSetLayoutBinding(const VulkanDescriptorSetLayout* layout)
    {
        if (layout == nullptr)
        {
            return;
        }

        m_setLayouts.push_back(layout);
    }
} // narc_engine
