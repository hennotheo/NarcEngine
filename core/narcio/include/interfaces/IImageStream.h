//
// Created by theo on 12/22/25.
//

#pragma once

namespace narc_io {
    class NARC_IO_API IImageStream
    {
    public:
        virtual ~IImageStream() = default;
        
        NARC_PURE_VIRTUAL_GETTER(int, getWidth);
        NARC_PURE_VIRTUAL_GETTER(int, getHeight);
        NARC_PURE_VIRTUAL_GETTER(int, getChannels);
        NARC_PURE_VIRTUAL_GETTER(void*, getData);
    };
}