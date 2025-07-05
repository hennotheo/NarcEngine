//
// Created by theoh on 08/03/2025.
//

#pragma once

#include "resources/Resource.h"

namespace narc_engine
{
    class ResourceManager;
    class Shader;

    class NARC_ENGINE_API Material final : public Resource
    {
        friend class ResourceManager;

    public:
        ~Material() override;

        NARC_GETTER(ResourceId, getMainTexture, m_mainTexture);
        NARC_GETTER(ResourceId, getShader, m_shader);

    protected:
        explicit Material(const char* texturePath);

        void onLoad() override {};
        void onUnload() override {};

    private:
        static uint32_t s_materialCounter;

        ResourceId m_shader;
        ResourceId m_mainTexture;
    };
} // narc_engine
