#pragma once

namespace narc_engine
{
    typedef const std::vector<const char *>* LayersPtr;

    class EngineInstance;
    class ISurfaceProvider;
    class EngineDebugLogger;
    class Engine;

    class EngineBuilder final
    {
        friend class Engine;

    public:
        GETTER const EngineInstance* getInstance() const { return m_instance; }
        GETTER const EngineDebugLogger* getDebugLogger() const { return m_debugLogger; }
        GETTER const ISurfaceProvider* getSurfaceProvider() const { return m_surface; }
        GETTER LayersPtr getValidationLayers() const { return m_validationLayers; }
        GETTER LayersPtr getDeviceExtensions() const { return m_deviceExtensions; }

        inline void setValidationLayers(LayersPtr validationLayers) { m_validationLayers = validationLayers; }
        inline void setDeviceExtensions(LayersPtr extensions) { m_deviceExtensions = extensions; }

    private:
        LayersPtr m_validationLayers;
        LayersPtr m_deviceExtensions;

        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;
        const EngineDebugLogger* m_debugLogger = nullptr;
    };

} // narc_engine