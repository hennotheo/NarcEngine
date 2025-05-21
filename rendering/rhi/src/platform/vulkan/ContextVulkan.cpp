#include "platform/vulkan/ContextVulkan.h"

namespace narc_engine
{
#define MINIMUM_VK_VERSION VK_API_VERSION_1_1
    ContextVulkan::ContextVulkan()
    {
        m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        m_appInfo.pApplicationName = "No Name Application";
        m_appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        m_appInfo.pEngineName = "NarcEngine";
        m_appInfo.engineVersion = VK_MAKE_VERSION(0, 2, 0);
        m_appInfo.apiVersion = VK_API_VERSION_1_1;

        m_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_createInfo.pApplicationInfo = &m_appInfo;
    }

    ContextVulkan::~ContextVulkan()
    {

    }

    void ContextVulkan::init()
    {
        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create instance!");
        }
    }

    void ContextVulkan::shutdown()
    {
        vkDestroyInstance(m_instance, nullptr);
    }

    void ContextVulkan::setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch)
    {
        m_appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);
    }

    void ContextVulkan::setApplicationName(const char* name)
    {
        m_appInfo.pApplicationName = name;
    }
} // namespace narc_engine
