#pragma once

#include "rhi/ContextRhi.h"

#include "context/ExtensionVulkan.h"

namespace narc_engine
{
    class ContextVulkan final : public ContextRhi
    {
    public:
        ContextVulkan();
        ~ContextVulkan() override;

        // Add move constructor and assignment operator
        ContextVulkan(ContextVulkan&&) = default;
        ContextVulkan& operator=(ContextVulkan&&) = default;

        // Explicitly delete copy operations
        ContextVulkan(const ContextVulkan&) = delete;
        ContextVulkan& operator=(const ContextVulkan&) = delete;

        NARC_IMPL_INITIALISABLE();
        NARC_IMPL_RHI_PLATFORM_GETTER(Context, Vulkan)

        NARC_OVERRIDE_GETTER(RendererApiType, getRendererApiType, RendererApiType::Vulkan);
        NARC_GETTER(const VkInstance&, getVkInstance, m_instance);

        QUERY ApplicationInfos getApplicationInfos() const noexcept override;

        void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch) override;
        void setApplicationName(const char* name) override;

    private:
        VkInstanceCreateInfo m_createInfo{};
        VkInstance m_instance{};
        VkApplicationInfo m_appInfo{};

        std::vector<std::unique_ptr<ExtensionVulkan>> m_requiredExtensions{};
        std::vector<const char*> m_requiredLayers{};

        RhiResult enableExtension(const RhiExtension& extension) override;
        RhiResult enableLayer(const RhiLayer& extension) override;
    };
} // namespace narc_engine
