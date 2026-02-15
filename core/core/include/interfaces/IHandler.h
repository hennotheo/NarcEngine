//
// Created by theo on 2/15/26.
//

#pragma once

namespace narc_core
{
    class IHandler
    {
    public:
        virtual ~IHandler() = default;

        NARC_PURE_VIRTUAL_GETTER(void*, getHandle);
    };
} // namespace