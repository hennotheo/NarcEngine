//
// Created by theoh on 5/23/2025.
//

#pragma once
#include "platform/vulkan/VulkanUtils.h"

namespace narc_engine
{
    class ContextVulkan;

    RhiResult isExtensionSupported();

    class ExtensionVulkan : public narc_core::IInitialisable
    {
    public:
        explicit ExtensionVulkan(const ContextVulkan* ctx, const char* name);
        ~ExtensionVulkan() override = default;

        GETTER const char* getName() const { return m_name; }

        VIRTUAL_GETTER const void* getCreateInfo() const = 0;

        virtual void linkNextCreateInfo(const void* next) = 0;

    protected:
        const ContextVulkan* m_context = nullptr;

    private:
        const char* m_name = nullptr;
    };

    class DebugExtensionVulkan final : public ExtensionVulkan
    {
    public:
        explicit DebugExtensionVulkan(const ContextVulkan* ctx);
        ~DebugExtensionVulkan() override;

        GETTER const void* getCreateInfo() const override { return &m_createInfo; }

        inline void linkNextCreateInfo(const void* next) override { m_createInfo.pNext = next; }

    protected:
        NARC_IMPL_INITIALISABLE();

    private:
        VkDebugUtilsMessengerCreateInfoEXT m_createInfo{};
        VkDebugUtilsMessengerEXT m_debugMessenger{};
    };

#define NARC_DECL_EXTENSION_VULKAN(name, vkName) \
    class name##ExtensionVulkan final : public ExtensionVulkan \
    {\
    public:\
        explicit name##ExtensionVulkan(const ContextVulkan* ctx) : ExtensionVulkan(ctx, vkName){};\
        ~name##ExtensionVulkan() override{}\
        \
        GETTER const void* getCreateInfo() const override { return nullptr; }\
        \
        inline void linkNextCreateInfo(const void* next) override {};\
        void init() override {}\
        void shutdown() override {}\
    }

    NARC_DECL_EXTENSION_VULKAN(Surface, VK_KHR_SURFACE_EXTENSION_NAME);
    NARC_DECL_EXTENSION_VULKAN(ExtendedDevicesProperties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    NARC_DECL_EXTENSION_VULKAN(ExtendedSurfaceCapabilities, VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
}
