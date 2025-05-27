#pragma once

namespace narc_engine
{
    class EngineInstance;
    class ISurfaceProvider;
    class EngineDebugLogger;
    class PhysicalDevice;
    class LogicalDevice;
    class Engine;

    class EngineBuilder final
    {
        friend class Engine;

    public:
        NARC_GETTER(const EngineInstance*, getInstance, m_instance);
        NARC_GETTER(const EngineDebugLogger*, getDebugLogger, m_debugLogger);
        NARC_GETTER(const ISurfaceProvider*, getSurfaceProvider, m_surface);
        NARC_GETTER(const PhysicalDevice*, getPhysicalDevice, m_physicalDevice);
        NARC_GETTER(const LogicalDevice*, getLogicalDevice, m_logicalDevice);
        NARC_GETTER(const KeywordList*, getValidationLayers, m_validationLayers);
        NARC_GETTER(const KeywordList*, getDeviceExtensions, m_deviceExtensions);
        NARC_GETTER(uint32_t, getFrameInFlightCount, m_frameInFlightCount);

        inline void setValidationLayers(const KeywordList* validationLayers) { m_validationLayers = validationLayers; }
        inline void setDeviceExtensions(const KeywordList* extensions) { m_deviceExtensions = extensions; }

    PRIVATE_TESTABLE:
        const KeywordList* m_validationLayers;
        const KeywordList* m_deviceExtensions;

        const uint32_t m_frameInFlightCount = 2;

        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;
        const EngineDebugLogger* m_debugLogger = nullptr;
        const PhysicalDevice* m_physicalDevice = nullptr;
        const LogicalDevice* m_logicalDevice = nullptr;
    };
} // narc_engine
