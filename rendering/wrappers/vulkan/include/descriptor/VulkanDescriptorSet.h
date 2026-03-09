//
// Created by theo on 12/20/25.
//

#pragma once

namespace narc_engine {
    class VulkanDescriptorSet : public IDescriptorBinding
    {
    public:
        explicit VulkanDescriptorSet(const VulkanDevice* device, const IDescriptorLayout* layout, const VkDescriptorSet& descriptorSet);
        ~VulkanDescriptorSet() override;

        NARC_OVERRIDE_GETTER(const IDescriptorLayout*, getLayout, m_layout);
        NARC_GETTER(VkDescriptorSet, getHandle, m_descriptorSet)
        std::unique_ptr<IDescriptorBindingUpdater> createUpdater() override;
        
    private:
        const IDescriptorLayout* m_layout;
        const VulkanDevice* m_device;

        VkDescriptorSet m_descriptorSet;
    };
} // narc_engine
