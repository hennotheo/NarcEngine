//
// Created by theo on 2/28/26.
//

#pragma once

namespace narc_engine {
    struct ImageAllocationInfo
    {
        ImageAllocationInfo() = default;

        MemorySize Size{};
        std::string Path{};
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