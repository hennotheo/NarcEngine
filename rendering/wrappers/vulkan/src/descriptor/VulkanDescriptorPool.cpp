//
// Created by theo on 12/20/25.
//

#include "descriptor/VulkanDescriptorPool.h"

#include "device/VulkanDevice.h"
#include "descriptor/VulkanDescriptorSet.h"
#include "descriptor/VulkanDescriptorSetLayout.h"

namespace narc_engine {
    VulkanDescriptorPool::VulkanDescriptorPool(const VulkanDevice* device) :
        m_device(device)
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

        if (vkCreateDescriptorPool(m_device->getHandle(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            NARC_LOG_FATAL("failed to create descriptor pool!");
        }
    }

    void VulkanDescriptorPool::shutdown()
    {
        vkDestroyDescriptorPool(m_device->getHandle(), m_descriptorPool, nullptr);
    }

    std::vector<std::unique_ptr<IDescriptorBinding>> VulkanDescriptorPool::allocateDescriptorSet(
            std::span<const VulkanDescriptorSetLayout*> layouts) const
    {
        const auto layoutCount = static_cast<uint32_t>(layouts.size());

        std::vector<VkDescriptorSetLayout> vkLayouts;
        vkLayouts.reserve(layoutCount);
        std::ranges::transform(layouts,
                               std::back_inserter(vkLayouts),
                               [](const VulkanDescriptorSetLayout* layout) {
                                   return layout->getHandle();
                               });

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.descriptorSetCount = vkLayouts.size();
        allocInfo.pSetLayouts = vkLayouts.data();

        std::vector<VkDescriptorSet> descriptorSets(m_descriptorCount);
        vkAllocateDescriptorSets(m_device->getHandle(), &allocInfo, descriptorSets.data());

        std::vector<std::unique_ptr<IDescriptorBinding>> bindings;
        bindings.reserve(layouts.size());
        for (int i = 0; i < layouts.size(); ++i)
        {
            bindings.push_back(std::make_unique<VulkanDescriptorSet>(m_device, layouts[i], descriptorSets[i]));
        }

        return bindings;
    }
} // narc_engine
