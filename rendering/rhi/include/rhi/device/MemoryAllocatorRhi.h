//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "ContextRhi.h"
#include "DeviceRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(MemoryAllocator);

    class NARC_ENGINE_API MemoryAllocatorRhi : public narc_core::IInitialisable
    {
    public:
        MemoryAllocatorRhi();

        NARC_DECL_RHI_PLATFORM_GETTERS(MemoryAllocator);
    };

    using MemoryAllocatorRhiPtr = std::shared_ptr<MemoryAllocatorRhi>;
}
