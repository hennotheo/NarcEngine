#pragma once

#include "rhi/ContextRhi.h"

namespace narc_engine
{
    class ContextVulkan final : public ContextRhi
    {
    public:
        ContextVulkan();
        ~ContextVulkan() override;

        NARC_IMPL_INITIALISABLE();

        void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch) override;
        void setApplicationName(const char* name) override;

    protected:
        RhiResult enableExtension(const RhiExtension& extension) override;
        RhiResult enableLayer(const RhiLayer& extension) override;

    private:
        VkInstanceCreateInfo m_createInfo{};
        VkInstance m_instance{};
        VkApplicationInfo m_appInfo{};

        std::vector<const char*> m_requiredExtensions{};
        std::vector<const char*> m_requiredLayers{};
    };
} // namespace narc_engine
