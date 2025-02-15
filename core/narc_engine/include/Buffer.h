#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

namespace narc_engine
{
    template <class T>
    class Buffer
    {
    public:
        VkBuffer getBuffer() const { return m_buffer; }
        
        void create(const std::vector<T>& input, VkBufferUsageFlagBits usage);

        void release();

    private:
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

        VkDevice m_linkedDevice;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    };
}
