#pragma once

#include <vulkan/vulkan.h>

#include "resources/Resource.h"

#include "platform/vulkan/ShaderModule.h"

namespace narc_engine {
    class Shader final : public Resource
    {
    public:
        ~Shader() override;

        TEMP_CODE GETTER VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }

        VkPipelineShaderStageCreateInfo configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const;

    protected:
        friend class ResourceManager;

        explicit Shader(const std::string& vertexShaderFile, const std::string& fragShaderFile);

        void onLoad() override;
        void onUnload() override;

    private:
        ShaderModule m_fragShaderModule;
        ShaderModule m_vertexShaderModule;
        VkDescriptorSetLayout m_descriptorSetLayout;

    private:
        void createDescriptorSetLayout();
    };
} // narc_engine
