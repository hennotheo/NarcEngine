//
// Created by theoh on 3/14/2025.
//

#include "models/ShaderModule.h"

#include <NarcIO.h>
#include <NarcLog.h>

#include "Engine.h"

namespace narc_engine {
    ShaderModule::ShaderModule(const std::string& filename)
    {
        m_device = Engine::getInstance()->getDevice();

        const auto code = narc_io::FileReader::readFile(filename);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(m_device->getDevice(), &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create shader module!");
        }
    }

    ShaderModule::~ShaderModule()
    {
        vkDestroyShaderModule(m_device->getDevice(), m_shaderModule, nullptr);
    }

    VkPipelineShaderStageCreateInfo ShaderModule::configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const
    {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = stage;
        vertShaderStageInfo.module = m_shaderModule;
        vertShaderStageInfo.pName = entryPoint;

        return vertShaderStageInfo;
    }
} // narc_engine
