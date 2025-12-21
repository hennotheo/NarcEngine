//
// Created by theo on 12/19/25.
//

#pragma once

namespace narc_engine {
    class VulkanDescriptorSetLayout : narc_core::IInitialisable
    {
    public:
        explicit VulkanDescriptorSetLayout(NARC_DI_IMPORT_COMPONENT(VulkanDevice));
        ~VulkanDescriptorSetLayout() override;
        
        NARC_IMPL_INITIALISABLE();
        
        NARC_GETTER(VkDescriptorSetLayout, getHandle, m_descriptorSetLayout);
        
    private:
        narc_core::injected_component<VulkanDevice> m_device;
        
        VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
    };
} // narc_engine
