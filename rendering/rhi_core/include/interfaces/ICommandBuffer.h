//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class ICommandBuffer
    {
    public:
        virtual ~ICommandBuffer() = default;

        NARC_PURE_VIRTUAL_CMD(begin);
        NARC_PURE_VIRTUAL_CMD(end);
        NARC_PURE_VIRTUAL_CMD(copyBuffer, const ICommandBuffer* source, const ICommandBuffer* destination);
    };
}