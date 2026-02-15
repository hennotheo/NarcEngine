#pragma once

#include "macros/GetterSetter.h"

namespace narc_core
{
#define NARC_IMPL_IGETTER(T, value) NARC_OVERRIDE_GETTER(T, get, value)

    template<typename T>
    class IGetter
    {
    public:
        virtual ~IGetter() = default;

        NARC_PURE_VIRTUAL_GETTER(T, get);
    };
} // namespace 