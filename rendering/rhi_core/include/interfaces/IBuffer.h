//
// Created by theo on 2/26/26.
//

#pragma once

namespace narc_engine {
    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;

        virtual void setData(const void* data) = 0;
        NARC_PURE_VIRTUAL_GETTER(MemorySize, getSize);
    };
}