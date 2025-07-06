//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "DeviceRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(GraphicsQueue);

    class NARC_ENGINE_API GraphicsQueueRhi : public narc_core::IInitialisable
    {
    public:
        explicit GraphicsQueueRhi(const DeviceRhiPtr& device);
        ~GraphicsQueueRhi() override = default;

        NARC_DECL_RHI_PLATFORM_GETTERS(GraphicsQueue);

        virtual void submit() = 0;
        virtual void waitIdle() = 0;

    protected:
        NARC_VIRTUAL_GETTER(DeviceRhi*, getDevice, m_device.get())

    private:
        const DeviceRhiPtr m_device;
    };

    using GraphicsQueueRhiPtr = std::shared_ptr<GraphicsQueueRhi>;
} // narc_engine