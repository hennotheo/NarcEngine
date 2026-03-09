//
// Created by theo on 3/6/26.
//
#include "descriptor/VulkanDescriptorBindingUpdater.h"

#include "VulkanTextureImage.h"
#include "descriptor/VulkanDescriptorSet.h"
#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanDescriptorBindingUpdater::VulkanDescriptorBindingUpdater(const VulkanDevice* device, const VulkanDescriptorSet* set) :
        m_set(set), m_device(device)
    {
    }

    VulkanDescriptorBindingUpdater::~VulkanDescriptorBindingUpdater() = default;

    void VulkanDescriptorBindingUpdater::updateBuffer(uint32_t bindingIndex, const IBuffer* buffer)
    {
        const auto vkBuffer = narc_core::backend_cast<IVulkanBuffer>(buffer);

        VkDescriptorBufferInfo infos{};
        infos.buffer = vkBuffer->getHandle();
        infos.offset = 0;
        infos.range = vkBuffer->getSize();
        m_buffers.push_back({bindingIndex, infos});
    }

    void VulkanDescriptorBindingUpdater::updateImageSampler(uint32_t bindingIndex, const IImage* image)
    {
        const auto vkTexture = narc_core::backend_cast<VulkanTextureImage>(image);

        VkDescriptorImageInfo infos{};
        infos.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        infos.imageView = vkTexture->getView();
        infos.sampler = vkTexture->getSampler();
        m_samplers.push_back({bindingIndex, infos});
    }

    void VulkanDescriptorBindingUpdater::update()
    {
        std::vector<VkWriteDescriptorSet> writes;
        for (const auto& [binding, imageInfos]: m_samplers)
        {
            VkWriteDescriptorSet write{};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = m_set->getHandle();
            write.dstBinding = binding;
            write.dstArrayElement = 0;
            write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            write.descriptorCount = 1;
            write.pImageInfo = &imageInfos;
            writes.push_back(write);
        }

        for (const auto& [binding, bufferInfos]: m_buffers)
        {
            VkWriteDescriptorSet write{};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = m_set->getHandle();
            write.dstBinding = binding;
            write.dstArrayElement = 0;
            write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            write.descriptorCount = 1;
            write.pBufferInfo = &bufferInfos;
            write.pImageInfo = nullptr;
            write.pTexelBufferView = nullptr;
            writes.push_back(write);
        }

        vkUpdateDescriptorSets(m_device->getHandle(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
    }
}
