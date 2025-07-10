//
// Created by theoh on 25/05/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(Device)

    class NARC_ENGINE_API DeviceRhi : public narc_core::IInitialisable
    {
    public:
        DeviceRhi();
        ~DeviceRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(Device);

        virtual void waitIdle() = 0;
    };

    using DeviceRhiPtr = std::shared_ptr<DeviceRhi>;
}
