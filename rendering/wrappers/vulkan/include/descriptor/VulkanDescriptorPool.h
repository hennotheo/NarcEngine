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
        explicit VulkanDescriptorPool(NARC_DI_IMPORT_COMPONENT(VulkanDevice));
        ~VulkanDescriptorPool() override;
        
        NARC_IMPL_INITIALISABLE();
        
        NARC_SETTER(uint32_t, DescriptorCount, m_descriptorCount);
        
        NO_DISCARD std::vector<VulkanDescriptorSet> allocateDescriptorSet(std::vector<VulkanDescriptorSetLayout> layouts);
        
    private:
        narc_core::injected_component<VulkanDevice> m_device;
        
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
        
        uint32_t m_descriptorCount = 0;
    };
} // narc_engine
