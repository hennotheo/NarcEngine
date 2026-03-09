//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class IImage : public narc_core::IInitialisable
    {
    public:
        ~IImage() override = default;

        NARC_PURE_VIRTUAL_GETTER(narc_math::Extent, getExtent);
    };
}