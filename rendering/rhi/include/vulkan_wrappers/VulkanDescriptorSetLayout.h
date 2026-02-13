//
// Created by theo on 12/19/25.
//

#pragma once

namespace narc_engine {
    enum ShaderStage
    {
        Vertex = 0b0001,
        Fragment = 0b0010
    };

    enum DescriptorType
    {
        Sampler,
        UniformBuffer
    };

    struct DescriptorSetBindingInfo
    {
        uint32_t BindingIndex;
        ShaderStage Stage;
        DescriptorType Type;
    };

    class VulkanDescriptorSetLayout : narc_core::IInitialisable
    {
    public:
        explicit VulkanDescriptorSetLayout(NARC_DI_IMPORT_COMPONENT(VulkanDevice));
        ~VulkanDescriptorSetLayout() override;
        
        NARC_IMPL_INITIALISABLE();

        void addBinding(DescriptorSetBindingInfo binding);

        NARC_GETTER(VkDescriptorSetLayout, getHandle, m_descriptorSetLayout);
        
    private:
        narc_core::injected_component<VulkanDevice> m_device;
        
        VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

        std::vector<DescriptorSetBindingInfo> m_bindings;

        VkDescriptorSetLayoutBinding mapFromDescriptorSetBindingInfo(DescriptorSetBindingInfo binding);
        VkShaderStageFlags mapFromShaderStage(ShaderStage stage);
        VkDescriptorType mapFromDescriptorType(DescriptorType type);
    };
} // narc_engine
