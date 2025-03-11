#pragma once
#include "Buffer.h"
#include "models/UniformBufferObject.h"

namespace narc_engine
{
    class UniformBuffer final : public Buffer
    {
    public:
        explicit UniformBuffer(const VkDeviceSize& bufferSize);

        void setData(const UniformBufferObject& ubo);
    private:
        void* uniformBuffersMapped = nullptr;
    };
}
