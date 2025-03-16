//
// Created by theoh on 3/14/2025.
//

#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"

namespace narc_engine {
    class DeviceHandler;

    class ShaderModule : public DeviceComponent
    {
    public:
        explicit ShaderModule(const std::string& filename);
        ~ShaderModule();

        VkPipelineShaderStageCreateInfo configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const;

    private:
        VkShaderModule m_shaderModule;
    };
} // narc_engine
