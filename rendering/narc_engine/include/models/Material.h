//
// Created by theoh on 08/03/2025.
//

#pragma once

#include "resources/GraphicResourceHandler.h"

namespace narc_engine
{
    class NARC_ENGINE_API Material
    {
    public:
        explicit Material(const char* texturePath);
        ~Material();

        [[nodiscard]] const uint32_t& getMaterialID() const { return m_materialID; }
        [[nodiscard]] const GraphicResourceHandler& getMainTexture() const { return m_mainTexture; }

    private:
        static uint32_t s_materialCounter;

        uint32_t m_materialID;

        GraphicResourceHandler m_mainTexture;
    };
} // narc_engine
