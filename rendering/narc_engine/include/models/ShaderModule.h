//
// Created by theoh on 3/14/2025.
//

#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine {
    class DeviceHandler;

    class ShaderModule
    {
    public:
        explicit ShaderModule(const std::string& filename);
        ~ShaderModule();

        VkPipelineShaderStageCreateInfo configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const;

    private:
        VkShaderModule m_shaderModule;

        const DeviceHandler* m_device;
    };
} // narc_engine
