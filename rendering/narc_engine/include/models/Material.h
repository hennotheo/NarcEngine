//
// Created by theoh on 08/03/2025.
//

#pragma once

#include "resources/GraphicResourceHandler.h"

namespace narc_engine
{
    class Shader;

    class NARC_ENGINE_API Material
    {
    public:
        explicit Material(const char* texturePath);
        ~Material();

        GETTER const uint32_t& getMaterialID() const { return m_materialID; }
        GETTER const GraphicResourceHandler& getMainTexture() const { return m_mainTexture; }

        GETTER const Shader* getVertShader() const { return m_vertShaderModule; }
        GETTER const Shader* getFragShader() const { return m_fragShaderModule; }

    private:
        static uint32_t s_materialCounter;

        uint32_t m_materialID;

        const Shader* m_vertShaderModule;
        const Shader* m_fragShaderModule;
        GraphicResourceHandler m_mainTexture;
    };
} // narc_engine
