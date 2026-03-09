//
// Created by theo on 12/20/25.
//

#pragma once

namespace narc_engine {
    class VulkanDescriptorSetLayout;
    class VulkanDescriptorSet;
    class VulkanDevice;
    
    class VulkanDescriptorPool : narc_core::IInitialisable
    {
    public:
        explicit VulkanDescriptorPool(const VulkanDevice* device);
        ~VulkanDescriptorPool() override;
        
        NARC_IMPL_INITIALISABLE();
        
        NARC_SETTER(uint32_t, setDescriptorCount, m_descriptorCount);
        
        NO_DISCARD std::vector<std::unique_ptr<IDescriptorBinding>> allocateDescriptorSet(std::span<const VulkanDescriptorSetLayout*> layouts) const;
        
    private:
        const VulkanDevice* m_device;
        
        VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
        
        uint32_t m_descriptorCount = 0;
    };
} // narc_engine
