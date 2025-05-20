#pragma once
#include "Buffer.h"
#include "models/UniformBufferObject.h"

namespace narc_engine
{
    class UniformBuffer final : public Buffer
    {
    public:
        UniformBuffer();
        ~UniformBuffer() override = default;

        GETTER const VkDeviceSize getUniformBufferSize(uint32_t index) const { return m_bufferSizes[index]; }
        QUERY const VkDeviceSize getValidUniformBufferSize(VkDeviceSize size) const;

        void beginRegister(VkDeviceSize totalSize);
        void registerBufferObject(void* data, VkDeviceSize size);
        void endRegister();

    private:
        void* m_uniformBuffersMapped = nullptr;
        VkDeviceSize m_size;
        VkDeviceSize m_minBufferObjectSize;

        std::vector<VkDeviceSize> m_bufferSizes;
        VkDeviceSize m_currentRegisteredSize = 0;

    private:
        void grow(VkDeviceSize newSize);
        void recreate();
    };
}
