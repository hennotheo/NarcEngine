//
// Created by theoh on 3/15/2025.
//

#pragma once
#include "DeviceHandler.h"

namespace narc_engine
{
    class DeviceHandler;

    class DeviceComponent
    {
    public:
        DeviceComponent();

    protected:
        GETTER const DeviceHandler* getDeviceHandler() const { return m_device; }
        GETTER VkDevice getVkDevice() const { return m_device->getDevice(); }

    private:
        const DeviceHandler* m_device;
    };
}