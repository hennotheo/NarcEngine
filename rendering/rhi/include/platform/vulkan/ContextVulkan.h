#pragma once

#include "rhi/ContextRhi.h"

namespace narc_engine
{
    class ContextVulkan : public ContextRhi
    {
    public:
        ContextVulkan();
        ~ContextVulkan() override;

        void init() override;
        void shutdown() override;

        void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch) override;
        void setApplicationName(const char* name) override;

    private:
        VkInstanceCreateInfo m_createInfo{};
        VkInstance m_instance;
        VkApplicationInfo m_appInfo{};
    };
} // namespace narc_engine
