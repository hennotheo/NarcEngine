//
// Created by theoh on 3/15/2025.
//

#pragma once
#include "core/devices/DeviceHandler.h"

namespace narc_engine
{
    class DeviceHandler;

    class DeviceComponent
    {
    public:
        DeviceComponent();
        virtual ~DeviceComponent() = default;

    protected:
        GETTER const DeviceHandler* getDeviceHandler() const { return m_device; }
        GETTER VkDevice getVkDevice() const { return m_device->getLogicalDevice()->getVkDevice(); }

    private:
        const DeviceHandler* m_device;
    };
}