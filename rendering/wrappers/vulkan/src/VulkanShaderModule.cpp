//
// Created by theohenno on 11/27/25.
//

#include "VulkanShaderModule.h"

#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanShaderModule::VulkanShaderModule(const std::weak_ptr<VulkanDevice>& device, std::string path) :
        m_device(device),
        m_path(std::move(path))
    {
        
    }

    VulkanShaderModule::~VulkanShaderModule() = default;

    void VulkanShaderModule::init()
    {        
        const auto code = narc_io::FileReaderService::readFile(m_path);

        m_shaderModule = createShaderModule(code);
    }

    void VulkanShaderModule::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        vkDestroyShaderModule(device->getHandle(), m_shaderModule, nullptr);
        m_shaderModule = VK_NULL_HANDLE;
    }

    VkShaderModule VulkanShaderModule::createShaderModule(const std::vector<char>& code)
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device->getHandle(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create shader module!");
        }

        return shaderModule;
    }
} // narc_engine
