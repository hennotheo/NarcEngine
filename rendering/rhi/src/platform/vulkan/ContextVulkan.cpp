#include "platform/vulkan/ContextVulkan.h"

#include <vulkan/vulkan.h>

namespace narc_engine
{
#define MINIMUM_VK_VERSION VK_API_VERSION_1_1

    ContextVulkan::ContextVulkan()
    {
        m_requiredExtensions.reserve(10);

        m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        m_appInfo.pApplicationName = "No Name Application";
        m_appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        m_appInfo.pEngineName = "NarcEngine";
        m_appInfo.engineVersion = VK_MAKE_VERSION(0, 2, 0);
        m_appInfo.apiVersion = VK_API_VERSION_1_1;

        m_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_createInfo.pApplicationInfo = &m_appInfo;

        addExtensions(CoreExtension, 1);
    }

    ContextVulkan::~ContextVulkan() {}

    void ContextVulkan::init()
    {
        m_createInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredExtensions.size());
        m_createInfo.ppEnabledExtensionNames = m_requiredExtensions.data();

        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create instance!");
        }
    }

    void ContextVulkan::shutdown() { vkDestroyInstance(m_instance, nullptr); }

    void ContextVulkan::setApplicationVersion(const uint16_t major, const uint16_t minor, const uint16_t patch)
    {
        m_appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);
    }

    void ContextVulkan::setApplicationName(const char* name) { m_appInfo.pApplicationName = name; }

    RhiResult ContextVulkan::enableExtension(const RhiExtensions& extension)
    {
        switch (extension)
        {
        case RhiExtensions::Core:
            return RHI_SUCCESS;

        case RhiExtensions::DebugUtils:
            m_requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            return RHI_SUCCESS;

        case RhiExtensions::Surface:
            m_requiredExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            return RHI_SUCCESS;

        case RhiExtensions::ExtendedDevicesProperties:
            m_requiredExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            return RHI_SUCCESS;
        case RhiExtensions::ExtendedSurfaceCapabilities:
            m_requiredExtensions.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
            return RHI_SUCCESS;

        default:
            return RHI_FAILURE;
        }
    }
} // namespace narc_engine
