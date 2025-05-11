//
// Created by theoh on 08/03/2025.
//

#pragma once

#include "resources/Resource.h"

namespace narc_engine
{
    class Shader;

    class NARC_ENGINE_API Material final : public Resource
    {
    public:
        explicit Material(const char* texturePath);
        ~Material();

        GETTER ResourceId getMainTexture() const { return m_mainTexture; }

        GETTER const Shader* getVertShader() const { return m_vertShaderModule; }
        GETTER const Shader* getFragShader() const { return m_fragShaderModule; }

    protected:
        void onLoad() override {};
        void onUnload() override {};

    private:
        static uint32_t s_materialCounter;

        const Shader* m_vertShaderModule;
        const Shader* m_fragShaderModule;

        ResourceId m_mainTexture;
    };
} // narc_engine
