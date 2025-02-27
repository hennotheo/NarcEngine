#include "renderer/DescriptorPool.h"

#include "Engine.h"
#include <NarcLog.h>

namespace narc_engine {
    void DescriptorPool::create(DescriptorPoolBuilder* builder)
    {
        m_deviceHandler = Engine::getInstance()->getDevice();

        if (vkCreateDescriptorPool(m_deviceHandler->getDevice(), builder->build(), nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create descriptor pool!");
        }
    }

    void DescriptorPool::allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets) const
    {
        allocInfo->descriptorPool = m_descriptorPool;

        if (vkAllocateDescriptorSets(m_deviceHandler->getDevice(), allocInfo, descriptorSets) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to allocate descriptor sets!");
        }
    }

    void DescriptorPool::release()
    {
        vkDestroyDescriptorPool(m_deviceHandler->getDevice(), m_descriptorPool, nullptr);
    }
}
