#include "platform/vulkan/ShaderModule.h"

namespace narc_engine
{
    ShaderModule::ShaderModule(const std::string& filename)
    {
        const auto code = narc_io::FileReader::readFile(filename);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(NARC_DEVICE_HANDLE, &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create shader module!");
        }
    }

    ShaderModule::~ShaderModule()
    {
        vkDestroyShaderModule(NARC_DEVICE_HANDLE, m_shaderModule, nullptr);
    }
} // namespace narc_engine
