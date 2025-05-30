#pragma once

#include "InternalTypedefs.h"

namespace narc_engine
{
    class Buffer;
    class CommandPoolRhi;
    class PhysicalDevice;
    class LogicalDevice;

    void copyBuffer(const Buffer* srcBuffer, const Buffer* dstBuffer, DeviceSize size);

    const CommandPoolRhi* getCommandPool();
    const PhysicalDevice* getPhysicalDevice();
    const LogicalDevice* getLogicalDevice();
} // namespace narc_engine