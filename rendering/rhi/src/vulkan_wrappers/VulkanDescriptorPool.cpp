//
// Created by theo on 12/20/25.
//

#include "vulkan_wrappers/VulkanDescriptorPool.h"

#include "vulkan_wrappers/VulkanDevice.h"

namespace narc_engine {
    VulkanDescriptorPool::VulkanDescriptorPool(NARC_DI_IMPORT_COMPONENT(VulkanDevice)) :
        NARC_DI_IMPL_COMPONENT(VulkanDevice, m_device)
    {
    }

    VulkanDescriptorPool::~VulkanDescriptorPool() = default;

    void VulkanDescriptorPool::init()
    {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = m_descriptorCount;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = m_descriptorCount;

        if (vkCreateDescriptorPool(m_device->getHandle(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        {
            NARC_LOG_FATAL("failed to create descriptor pool!");
        }
    }

    void VulkanDescriptorPool::shutdown()
    {
        vkDestroyDescriptorPool(m_device->getHandle(), descriptorPool, nullptr);
    }
} // narc_engine
