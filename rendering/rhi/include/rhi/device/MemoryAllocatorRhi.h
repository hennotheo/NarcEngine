//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "DeviceRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(MemoryAllocator);

    class NARC_ENGINE_API MemoryAllocatorRhi : public narc_core::IInitialisable
    {
    public:
        explicit MemoryAllocatorRhi(const ContextRhiPtr& ctx, const DeviceRhiPtr& device);
        ~MemoryAllocatorRhi() override = default;

        NARC_DECL_RHI_PLATFORM_GETTERS(MemoryAllocator);

    protected:
        NARC_VIRTUAL_GETTER(DeviceRhi*, getDevice, m_device.get())
        NARC_VIRTUAL_GETTER(ContextRhi*, getContext, m_context.get())

    private:
        const ContextRhiPtr m_context;
        const DeviceRhiPtr m_device;
    };

    using MemoryAllocatorRhiPtr = std::shared_ptr<MemoryAllocatorRhi>;
}
