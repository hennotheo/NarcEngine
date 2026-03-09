//
// Created by theohenno on 11/28/25.
//

#pragma once

namespace narc_engine {
    class VulkanShaderModule;
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

        VulkanShaderModule createVertexShaderModule() const;
        VulkanShaderModule createFragmentShaderModule() const;
        NARC_GETTER(const VertexLayout&, getVertexLayout, m_vertexLayout);

        //RHI
        IPipelineLayout* setVertexShader(const std::string& path) override;
        IPipelineLayout* setFragmentShader(const std::string& path) override;
        IPipelineLayout* setVertexLayout(const VertexLayout& layout) override;
        IPipelineLayout* addBinding(const IDescriptorLayout* binding) override;

    private:
        const VulkanDevice* m_device;

        std::string m_vertexShaderPath{};
        std::string m_fragmentShaderPath{};
        VertexLayout m_vertexLayout{};

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        
        std::vector<const VulkanDescriptorSetLayout*> m_setLayouts;
    };
} // narc_engine
