//
// Created by theo on 12/20/25.
//

#pragma once

namespace narc_engine {
    class VulkanDescriptorSet
    {
    public:
        explicit VulkanDescriptorSet(const VkDescriptorSet& descriptorSet);
        ~VulkanDescriptorSet();
        
        NARC_GETTER(VkDescriptorSet, getHandle, m_descriptorSet);
        
    private:
        VkDescriptorSet m_descriptorSet;
    };
} // narc_engine
