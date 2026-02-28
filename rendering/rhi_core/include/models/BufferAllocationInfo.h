//
// Created by theo on 2/28/26.
//

#pragma once

namespace narc_engine {
    struct BufferAllocationInfo
    {
    public:
        BufferAllocationInfo();

        bool IsStaging { false };
    };
}