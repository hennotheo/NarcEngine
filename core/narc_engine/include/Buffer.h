#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "Vertex.h"

namespace NarcEngine
{
    class Buffer
    {
    public:
        void Create(const std::vector<Vertex>& Vertices);

        void Release();

        VkBuffer GetBuffer() const { return m_vertexBuffer; }

    private:
        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;

        VkDevice m_device;

        void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    };
}
