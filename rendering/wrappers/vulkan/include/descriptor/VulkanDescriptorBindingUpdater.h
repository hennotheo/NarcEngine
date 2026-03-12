//
// Created by theo on 3/6/26.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;
    class VulkanDescriptorSet;

    class VulkanDescriptorBindingUpdater : public IDescriptorBindingUpdater
    {
    public:
        explicit VulkanDescriptorBindingUpdater(const VulkanDevice* device, const VulkanDescriptorSet* set);
        ~VulkanDescriptorBindingUpdater() override;

        void updateBuffer(uint32_t bindingIndex, const IBuffer* buffer) override;
        void updateImageSampler(uint32_t bindingIndex, const IImage* image) override;
        void update() override;

    private:
        const VulkanDescriptorSet* m_set;
        const VulkanDevice* m_device;

        std::vector<std::pair<uint32_t, VkDescriptorBufferInfo>> m_buffers;
        std::vector<std::pair<uint32_t, VkDescriptorImageInfo >> m_samplers;
    };
}
