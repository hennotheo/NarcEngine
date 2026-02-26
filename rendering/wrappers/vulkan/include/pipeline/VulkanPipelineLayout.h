//
// Created by theohenno on 11/28/25.
//

#pragma once

namespace narc_engine {
    class VulkanDescriptorSetLayout;
    class VulkanDevice;

    class VulkanPipelineLayout final : public IPipelineLayout
    {
    public:
        explicit VulkanPipelineLayout(const VulkanDevice* device);
        ~VulkanPipelineLayout() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkPipelineLayout, getHandle, m_pipelineLayout);
        
        void addDescriptorSetLayoutBinding(const VulkanDescriptorSetLayout* layout);

    private:
        const VulkanDevice* m_device;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        
        std::vector<const VulkanDescriptorSetLayout*> m_setLayouts;
    };
} // narc_engine
