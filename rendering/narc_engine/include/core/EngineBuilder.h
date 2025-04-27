#pragma once

namespace narc_engine {
    class EngineInstance;
    
    typedef const std::vector<const char*>* ValidationLayersPtr;

    class EngineBuilder final
    {
    public:
        GETTER ValidationLayersPtr getValidationLayers() const { return m_validationLayers; }

        inline void setValidationLayers(ValidationLayersPtr validationLayers) { m_validationLayers = validationLayers; }

    private:
        ValidationLayersPtr m_validationLayers;
    };

} // narc_engine