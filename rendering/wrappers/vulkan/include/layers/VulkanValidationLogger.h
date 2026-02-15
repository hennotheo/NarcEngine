//
// Created by theohenno on 10/22/25.
//

#pragma once

#include "instance/VulkanInstance.h"

namespace narc_engine {
    class VulkanValidationLogger final : public IVulkanExtension
    {
    public:
        explicit VulkanValidationLogger(NARC_DI_IMPORT_COMPONENT(VulkanInstance));
        ~VulkanValidationLogger() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(ExtensionNameList, getExtensionNames, ExtensionNameList{VK_EXT_DEBUG_UTILS_EXTENSION_NAME})
        NARC_OVERRIDE_GETTER(const void*, getCreationInfos, &m_debugUtilsMessengerCreateInfo)

    private:
        narc_core::injected_component<VulkanInstance> m_instance;

        VkDebugUtilsMessengerCreateInfoEXT m_debugUtilsMessengerCreateInfo{};
        VkDebugUtilsMessengerEXT m_debugUtilsMessenger = VK_NULL_HANDLE;

        VkResult createVulkanUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* createInfos, const VkAllocationCallbacks* allocator);
        void destroyVulkanUtilsMessenger(const VkAllocationCallbacks* allocator);
    };
}
