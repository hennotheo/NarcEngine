//
// Created by theoh on 08/03/2025.
//

#pragma once

#include <NarcIO.h>

namespace narc_engine
{
    class NARC_ENGINE_API Material
    {
    public:
        explicit Material(const char* texturePath);
        ~Material();

        [[nodiscard]] const narc_io::Image& getMainTexture() const { return m_mainTexture; }

    private:
        narc_io::Image m_mainTexture;
    };
} // narc_engine
