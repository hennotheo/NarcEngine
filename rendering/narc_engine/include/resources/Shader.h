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

        explicit Shader(const char* vertexShaderFile, const char* fragShaderFile);

        void onLoad() override;
        void onUnload() override;

    private:
        ShaderModule m_vertexShaderModule;
        ShaderModule m_fragShaderModule;
        VkDescriptorSetLayout m_descriptorSetLayout;

    private:
        void createDescriptorSetLayout();
    };
} // narc_engine
