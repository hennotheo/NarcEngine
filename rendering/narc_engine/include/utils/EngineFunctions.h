#pragma once

#include "InternalTypedefs.h"

namespace narc_engine
{
    class Buffer;

    void copyBuffer(const Buffer* srcBuffer, const Buffer* dstBuffer, DeviceSize size);
} // namespace narc_engine