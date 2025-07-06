//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "device/DeviceRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES (PresentQueue);

    class NARC_ENGINE_API PresentQueueRhi : public narc_core::IInitialisable
    {
    public:
        explicit PresentQueueRhi(const DeviceRhiPtr& device);
        ~PresentQueueRhi() override = default;

        NARC_DECL_RHI_PLATFORM_GETTERS(PresentQueue);

        virtual void present() const = 0;

    protected:
        NARC_VIRTUAL_GETTER(DeviceRhi*, getDevice, m_device.get())

    private:
        const DeviceRhiPtr m_device;
    };

    using PresentQueueRhiPtr = std::shared_ptr<PresentQueueRhi>;
}
