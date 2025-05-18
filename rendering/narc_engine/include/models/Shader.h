#pragma once

#include <vulkan/vulkan.h>

#include "platform/vulkan/ShaderModule.h"

namespace narc_engine {
    class Shader
    {
    public:
        explicit Shader(const std::string& vertexShaderFile, const std::string& fragShaderFile);
        ~Shader();

        TEMP_CODE GETTER VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }

        VkPipelineShaderStageCreateInfo configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const;

    private:
        ShaderModule m_fragShaderModule;
        ShaderModule m_vertexShaderModule;
        VkDescriptorSetLayout m_descriptorSetLayout;

    private:
        void createDescriptorSetLayout();
    };
} // narc_engine
