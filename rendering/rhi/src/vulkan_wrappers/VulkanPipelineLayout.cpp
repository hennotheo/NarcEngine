//
// Created by theohenno on 11/28/25.
//

#include "vulkan_wrappers/VulkanPipelineLayout.h"

#include "vulkan_wrappers/VulkanDescriptorSetLayout.h"
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

    void VulkanPipelineLayout::addDescriptorSetLayoutBinding(const VulkanDescriptorSetLayout* layout)
    {
        if (layout == nullptr)
        {
            return;
        }

        m_setLayouts.push_back(layout);
    }
} // narc_engine
