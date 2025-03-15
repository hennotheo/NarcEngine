//
// Created by theoh on 3/15/2025.
//

#pragma once

#include <vulkan/vulkan.h>

#include "Engine.h"

namespace narc_engine {
    class DeviceHandler;

    class DeviceComponent
    {
    public:
        DeviceComponent()
        {
            // m_device = Engine::getInstance()->getDevice();
        }

    protected:
        GETTER const DeviceHandler* getDeviceHandler() const { return m_device; }
        GETTER VkDevice getVkDevice() const { return m_device->getDevice(); }

    private:
        const DeviceHandler* m_device;
    };
}
