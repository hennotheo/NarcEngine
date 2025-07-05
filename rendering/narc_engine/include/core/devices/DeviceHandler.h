#pragma once

#include "core/devices/PhysicalDevice.h"
#include "core/devices/LogicalDevice.h"

class ImGui_ImplVulkan_InitInfo;

namespace narc_engine
{
    class Window;
    class ISurfaceProvider;
    class EngineBuilder;

    class DeviceHandler
    {
    public:
        explicit DeviceHandler(const EngineBuilder* builder);
        ~DeviceHandler();

        NARC_GETTER(const PhysicalDevice*, getPhysicalDevice, m_physicalDevice.get())
        NARC_GETTER(const LogicalDevice*, getLogicalDevice, m_logicalDevice.get())

    private:
        std::unique_ptr<PhysicalDevice> m_physicalDevice;
        std::unique_ptr<LogicalDevice> m_logicalDevice;

        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;
        const KeywordList* m_deviceExtensions = nullptr;
    };
}
