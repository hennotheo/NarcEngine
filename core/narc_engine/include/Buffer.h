#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "Vertex.h"

namespace NarcEngine
{
    template <class T>
    class Buffer
    {
    public:
        VkBuffer GetBuffer() const { return m_vertexBuffer; }
        
        void Create(const std::vector<T>& input, VkBufferUsageFlagBits usage);

        void Release();

    private:
        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;

        VkDevice m_device;

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    };
}
