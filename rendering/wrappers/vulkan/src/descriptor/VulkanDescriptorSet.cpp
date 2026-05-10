//
// Created by theo on 12/20/25.
//

#include "descriptor/VulkanDescriptorSet.h"

#include "descriptor/VulkanDescriptorBindingUpdater.h"
#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice* device, const IDescriptorLayout* layout, const VkDescriptorSet& descriptorSet) :
        m_layout(layout),
        m_descriptorSet(descriptorSet),
        m_device(device)
    {
    }

    VulkanDescriptorSet::~VulkanDescriptorSet() = default;

    std::unique_ptr<IDescriptorBindingUpdater> VulkanDescriptorSet::createUpdater()
    {
        return std::make_unique<VulkanDescriptorBindingUpdater>(m_device, this);
    }
} // narc_engine
