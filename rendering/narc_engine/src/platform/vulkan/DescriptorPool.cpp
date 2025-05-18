#include "platform/vulkan/DescriptorPool.h"

#include "Engine.h"

namespace narc_engine {
    DescriptorPool::DescriptorPool(const uint32_t poolCount)
    {
        m_builder = std::make_unique<DescriptorPoolBuilder>(poolCount);
    }

    DescriptorPool::~DescriptorPool()
    {
        if (m_allocated)
            release();
    }

    void DescriptorPool::create()
    {
        if (vkCreateDescriptorPool(NARC_DEVICE_HANDLE, m_builder->build(), nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create descriptor pool!");
        }

        m_allocated = true;
    }

    void DescriptorPool::allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets) const
    {
        allocInfo->descriptorPool = m_descriptorPool;

        if (vkAllocateDescriptorSets(NARC_DEVICE_HANDLE, allocInfo, descriptorSets) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to allocate descriptor sets!");
        }
    }

    void DescriptorPool::release()
    {
        m_allocated = false;
        vkDestroyDescriptorPool(NARC_DEVICE_HANDLE, m_descriptorPool, nullptr);
    }
}
