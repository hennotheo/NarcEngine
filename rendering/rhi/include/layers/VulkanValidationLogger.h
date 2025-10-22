//
// Created by theohenno on 10/22/25.
//

#pragma once

#include "IVulkanExtension.h"

namespace narc_engine {
    class VulkanValidationLogger final : public IVulkanExtension
    {
    public:
        explicit VulkanValidationLogger(std::weak_ptr<VulkanInstance> instance);
        ~VulkanValidationLogger() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(constexpr const char*, getExtensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
        NARC_OVERRIDE_GETTER(const void*, getCreationInfos, &m_debugUtilsMessengerCreateInfo)

    private:
        VkDebugUtilsMessengerCreateInfoEXT m_debugUtilsMessengerCreateInfo;

        VkDebugUtilsMessengerEXT m_debugUtilsMessenger = VK_NULL_HANDLE;

        VkResult createVulkanUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* createInfos, const VkAllocationCallbacks* allocator);
        void destroyVulkanUtilsMessenger(const VkAllocationCallbacks* allocator);
    };
}
