//
// Created by theo on 12/19/25.
//

#include "descriptor/VulkanDescriptorSetLayout.h"

#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(NARC_DI_IMPORT_COMPONENT(VulkanDevice)) :
        NARC_DI_IMPL_COMPONENT(VulkanDevice, m_device)
    {
        //Empty constructor.
    }

    VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() = default;

    void VulkanDescriptorSetLayout::init()
    {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
        setLayoutBindings.reserve(m_bindings.size());

        std::ranges::transform(
                m_bindings,
                std::back_inserter(setLayoutBindings),
                [this](const auto& x) {
                    return mapFromDescriptorSetBindingInfo(x);
                });

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        layoutInfo.pBindings = setLayoutBindings.data();

        if (vkCreateDescriptorSetLayout(m_device->getHandle(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("failed to create descriptor set layout!");
        }
    }

    void VulkanDescriptorSetLayout::shutdown()
    {
        vkDestroyDescriptorSetLayout(m_device->getHandle(), m_descriptorSetLayout, nullptr);
    }

    void VulkanDescriptorSetLayout::addBinding(DescriptorSetBindingInfo binding)
    {
        m_bindings.push_back(binding);
    }

    VkDescriptorSetLayoutBinding VulkanDescriptorSetLayout::mapFromDescriptorSetBindingInfo(DescriptorSetBindingInfo binding)
    {
        return {
                .binding = binding.BindingIndex,
                .descriptorType = mapFromDescriptorType(binding.Type),
                .descriptorCount = 1,
                .stageFlags = mapFromShaderStage(binding.Stage),
                .pImmutableSamplers = nullptr
        };
    }

    VkShaderStageFlags VulkanDescriptorSetLayout::mapFromShaderStage(const ShaderStage stage)
    {
        VkShaderStageFlags flags = 0;

        if (stage == ShaderStage::Vertex)
        {
            flags |= VK_SHADER_STAGE_VERTEX_BIT;
        }

        if (stage == ShaderStage::Fragment)
        {
            flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
        }

        return flags;
    }

    VkDescriptorType VulkanDescriptorSetLayout::mapFromDescriptorType(const DescriptorType type)
    {
        switch (type)
        {
            case Sampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            case UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }

        NARC_ERROR_RUNTIME("Invalid descriptor type!");
    }
} // narc_engine
