//
// Created by theo on 2/28/26.
//

#pragma once

namespace narc_engine {
    struct BufferAllocationInfo
    {
        BufferAllocationInfo() = default;

        bool IsStaging{false};
        bool IsIndexBuffer{false};
        MemorySize Size{};
    };
}