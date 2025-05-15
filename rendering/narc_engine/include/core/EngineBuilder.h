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
        GETTER const EngineInstance* getInstance() const { return m_instance; }
        GETTER const EngineDebugLogger* getDebugLogger() const { return m_debugLogger; }
        GETTER const ISurfaceProvider* getSurfaceProvider() const { return m_surface; }
        GETTER const PhysicalDevice* getPhysicalDevice() const { return m_physicalDevice; }
        GETTER const LogicalDevice* getLogicalDevice() const { return m_logicalDevice; }
        GETTER const KeywordList* getValidationLayers() const { return m_validationLayers; }
        GETTER const KeywordList* getDeviceExtensions() const { return m_deviceExtensions; }
        GETTER uint32_t getFrameInFlightCount() const { return m_frameInFlightCount; }

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