#pragma once

namespace narc_engine
{
    typedef const std::vector<const char *> *LayersPtr;

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
        GETTER const EngineInstance *getInstance() const { return m_instance; }
        GETTER const EngineDebugLogger *getDebugLogger() const { return m_debugLogger; }
        GETTER const ISurfaceProvider *getSurfaceProvider() const { return m_surface; }
        GETTER const PhysicalDevice *getPhysicalDevice() const { return m_physicalDevice; }
        GETTER const LogicalDevice *getLogicalDevice() const { return m_logicalDevice; }
        GETTER LayersPtr getValidationLayers() const { return m_validationLayers; }
        GETTER LayersPtr getDeviceExtensions() const { return m_deviceExtensions; }

        inline void setValidationLayers(LayersPtr validationLayers) { m_validationLayers = validationLayers; }
        inline void setDeviceExtensions(LayersPtr extensions) { m_deviceExtensions = extensions; }

        PRIVATE_TESTABLE : LayersPtr m_validationLayers;
        LayersPtr m_deviceExtensions;

        const EngineInstance *m_instance = nullptr;
        const ISurfaceProvider *m_surface = nullptr;
        const EngineDebugLogger *m_debugLogger = nullptr;
        const PhysicalDevice *m_physicalDevice = nullptr;
        const LogicalDevice *m_logicalDevice = nullptr;
    };
} // narc_engine