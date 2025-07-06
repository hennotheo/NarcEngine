//
// Created by theoh on 7/6/2025.
//

#include "device/MemoryAllocatorRhi.h"

namespace narc_engine
{

    MemoryAllocatorRhi::MemoryAllocatorRhi(const ContextRhiPtr& ctx, const DeviceRhiPtr& device) : m_context(ctx), m_device(device)
    {
    }
}
