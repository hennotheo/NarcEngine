//
// Created by theohenno on 11/27/25.
//

#pragma once

#include "interfaces/IInitialisable.h"

namespace narc_engine {
    class VulkanDevice;
    class VulkanShaderModule final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanShaderModule(const std::weak_ptr<VulkanDevice>& device, std::string path);
        ~VulkanShaderModule() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(const VkShaderModule&, getHandle, m_shaderModule);
        
    private:
        std::weak_ptr<VulkanDevice> m_device;
        
        std::string m_path;

        VkShaderModule m_shaderModule = VK_NULL_HANDLE;

        VkShaderModule createShaderModule(const std::vector<char>& code);
    };
} // narc_engine
