//
// Created by theo on 12/20/25.
//

#include "descriptor/VulkanDescriptorSet.h"

namespace narc_engine {
    VulkanDescriptorSet::VulkanDescriptorSet(const IDescriptorLayout* layout, const VkDescriptorSet& descriptorSet) :
        m_layout(layout),
        m_descriptorSet(descriptorSet)
    {
        // const auto bindings = layout->
    }

    VulkanDescriptorSet::~VulkanDescriptorSet() = default;
} // narc_engine
