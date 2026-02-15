//
// Created by theo on 12/20/25.
//

#include "descriptor/VulkanDescriptorSet.h"

namespace narc_engine {
    VulkanDescriptorSet::VulkanDescriptorSet(const VkDescriptorSet& descriptorSet)
        : m_descriptorSet(descriptorSet)
    {
        //Empty constructor.
    }

    VulkanDescriptorSet::~VulkanDescriptorSet() = default;
} // narc_engine
