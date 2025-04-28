#pragma once

namespace narc_engine
{
    typedef const std::vector<const char *>* ValidationLayersPtr;

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
        GETTER ValidationLayersPtr getValidationLayers() const { return m_validationLayers; }

        inline void setValidationLayers(ValidationLayersPtr validationLayers) { m_validationLayers = validationLayers; }

    private:
        ValidationLayersPtr m_validationLayers;

        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;
        const EngineDebugLogger* m_debugLogger = nullptr;
    };

} // narc_engine