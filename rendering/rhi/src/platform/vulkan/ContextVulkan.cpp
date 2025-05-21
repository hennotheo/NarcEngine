#include "platform/vulkan/ContextVulkan.h"

namespace narc_engine
{
    void ContextVulkan::init()
    {
        NARCLOG_INFO("Vulkan context initialized");
    }

    void ContextVulkan::shutdown()
    {
        NARCLOG_INFO("Vulkan context shutdown");
    }
} // namespace narc_engine
