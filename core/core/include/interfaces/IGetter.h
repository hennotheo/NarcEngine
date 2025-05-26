#pragma once

#include "macros/GetterSetter.h"

namespace narc_core
{
#define NARC_IMPL_IGETTER(T, value)  GETTER const T& get() const noexcept override { return value; }

    template<typename T>
    class IGetter
    {
    public:
        virtual ~IGetter() = default;

        GETTER virtual const T& get() const noexcept = 0;
    };
} // namespace 