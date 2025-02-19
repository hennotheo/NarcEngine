#pragma once

namespace narc_engine {
    struct DescriptorPoolBuilder
    {
        friend class DescriptorPool;

        DescriptorPoolBuilder()
        {
            m_poolInfo = {};
            m_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            m_poolInfo.maxSets = 0;
        }

        inline void setMaxSet(const uint32_t maxSets)
        {
            m_poolInfo.maxSets = maxSets;
        }

        inline void addPoolSize(VkDescriptorType type, uint32_t count)
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type = type;
            poolSize.descriptorCount = count;

            m_poolSizes.push_back(poolSize);
        }

    protected:
        VkDescriptorPoolCreateInfo* build()
        {
            m_poolInfo.poolSizeCount = static_cast<uint32_t>(m_poolSizes.size());
            m_poolInfo.pPoolSizes = m_poolSizes.data();

            return &m_poolInfo;
        }

    private:
        VkDescriptorPoolCreateInfo m_poolInfo;

        std::vector<VkDescriptorPoolSize> m_poolSizes;
    };
}
