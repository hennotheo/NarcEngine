//
// Created by theo on 12/19/25.
//

#pragma once

namespace narc_engine {
    class VulkanDescriptorSetLayout : public IDescriptorLayout
    {
    public:
        explicit VulkanDescriptorSetLayout(const VulkanDevice* device);
        ~VulkanDescriptorSetLayout() override;
        
        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkDescriptorSetLayout, getHandle, m_descriptorSetLayout);

        NARC_OVERRIDE_GETTER(std::span<const DescriptorSetBindingInfo>, getBindings, m_bindings);
        void addBinding(const DescriptorSetBindingInfo& value) noexcept override;

    private:
        const VulkanDevice* m_device;

        VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

        std::vector<DescriptorSetBindingInfo> m_bindings;

        VkDescriptorSetLayoutBinding mapFromDescriptorSetBindingInfo(DescriptorSetBindingInfo binding);
        VkShaderStageFlags mapFromShaderStage(ShaderStage stage);
        VkDescriptorType mapFromDescriptorType(DescriptorType type);
    };
} // narc_engine
