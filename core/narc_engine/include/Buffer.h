#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

namespace NarcEngine
{
    template <class T>
    class Buffer
    {
    public:
        VkBuffer GetBuffer() const { return m_buffer; }
        
        void Create(const std::vector<T>& input, VkBufferUsageFlagBits usage);

        void Release();

    private:
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

        VkDevice m_linkedDevice;

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    };
}
