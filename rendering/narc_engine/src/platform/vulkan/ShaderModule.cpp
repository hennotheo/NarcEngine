#include "platform/vulkan/ShaderModule.h"

namespace narc_engine
{
    ShaderModule::ShaderModule(const char* filename) : m_filename(filename)
    {
    }

    ShaderModule::~ShaderModule()
    {
    }

    void ShaderModule::load()
    {
        const auto code = narc_io::FileReader::readFile(m_filename);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(NARC_DEVICE_HANDLE, &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create shader module!");
        }
    }

    void ShaderModule::unload()
    {
        vkDestroyShaderModule(NARC_DEVICE_HANDLE, m_shaderModule, nullptr);
    }
} // namespace narc_engine
