//
// Created by theo on 12/19/25.
//

#include "vulkan_wrappers/VulkanDescriptorSetLayout.h"

#include "vulkan_wrappers/VulkanDevice.h"

namespace narc_engine {
    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(NARC_DI_IMPORT_COMPONENT(VulkanDevice)) :
        NARC_DI_IMPL_COMPONENT(VulkanDevice, m_device)
    {
    }

    VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
    {
    }

    void VulkanDescriptorSetLayout::init()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(m_device->getHandle(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("failed to create descriptor set layout!");
        }
    }

    void VulkanDescriptorSetLayout::shutdown()
    {
        vkDestroyDescriptorSetLayout(m_device->getHandle(), m_descriptorSetLayout, nullptr);
    }
} // narc_engine
