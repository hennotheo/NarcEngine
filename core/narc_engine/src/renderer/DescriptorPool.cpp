#include "renderer/DescriptorPool.h"

#include "Engine.h"

namespace narc_engine {
    void DescriptorPool::create(DescriptorPoolBuilder* builder)
    {
        m_deviceHandler = Engine::getInstance()->getDevice();

        if (vkCreateDescriptorPool(m_deviceHandler->getDevice(), builder->build(), nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void DescriptorPool::allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets) const
    {
        allocInfo->descriptorPool = m_descriptorPool;

        if (vkAllocateDescriptorSets(m_deviceHandler->getDevice(), allocInfo, descriptorSets) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }

    void DescriptorPool::release()
    {
        vkDestroyDescriptorPool(m_deviceHandler->getDevice(), m_descriptorPool, nullptr);
    }
}
