//
// Created by theo on 2/28/26.
//

#pragma once

namespace narc_engine {
    struct ImageAllocationInfo
    {
        ImageAllocationInfo() = default;

        narc_math::Extent Extent{};
    };

    struct BufferAllocationInfo
    {
        BufferAllocationInfo() = default;

        bool IsStaging{false};
        bool IsIndexBuffer{false};
        bool IsVertexBuffer{false};
        MemorySize Size{};
    };
}