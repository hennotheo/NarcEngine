//
// Created by theo on 12/20/25.
//

#include "descriptor/VulkanDescriptorPool.h"

#include "device/VulkanDevice.h"
#include "descriptor/VulkanDescriptorSet.h"
#include "descriptor/VulkanDescriptorSetLayout.h"

namespace narc_engine {
    VulkanDescriptorPool::VulkanDescriptorPool(NARC_DI_IMPORT_COMPONENT(VulkanDevice)) :
        NARC_DI_IMPL_COMPONENT(VulkanDevice, m_device)
    {
        //Empty Constructor
    }

    VulkanDescriptorPool::~VulkanDescriptorPool() = default;

    void VulkanDescriptorPool::init()
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = m_descriptorCount;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = m_descriptorCount;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
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

    std::vector<VulkanDescriptorSet> VulkanDescriptorPool::allocateDescriptorSet(std::vector<VulkanDescriptorSetLayout> layouts)
    {
        const auto layoutCount = static_cast<uint32_t>(layouts.size());

        std::vector<VkDescriptorSetLayout> out;
        out.reserve(layoutCount);
        std::ranges::transform(layouts,
                               std::back_inserter(out),
                               [](const VulkanDescriptorSetLayout& layout) {
                                   return layout.getHandle();
                               });

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = m_descriptorCount;
        allocInfo.pSetLayouts = out.data();

        std::vector<VkDescriptorSet> descriptorSets;
        descriptorSets.resize(m_descriptorCount);
        vkAllocateDescriptorSets(m_device->getHandle(), &allocInfo, descriptorSets.data());

        std::vector<VulkanDescriptorSet> sets;
        sets.reserve(m_descriptorCount);
        std::ranges::transform(descriptorSets,
                               std::back_inserter(sets),
                               [](const VkDescriptorSet& set) {
                                   return VulkanDescriptorSet(set);
                               });

        return sets;
    }
} // narc_engine
