#include "platform/vulkan/DeviceMemory.h"

namespace narc_engine
{
    DeviceMemory::DeviceMemory()
    {
        m_allocated = false;
        m_memory = VK_NULL_HANDLE;
        m_allocInfo = {};
        m_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    }

    DeviceMemory::~DeviceMemory()
    {
        if (m_allocated)
        {
            NARCLOG_WARNING("Device memory not released!");
            release();
        }
    }

    void DeviceMemory::allocate()
    {
        if (m_allocated)
        {
            NARCLOG_WARNING("Device memory already allocated!");
            NARCLOG_WARNING(m_memory);
            return;
        }

        m_allocated = true;
        if (vkAllocateMemory(getVkDevice(), &m_allocInfo, nullptr, &m_memory) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to allocate image memory!");
        }
    }

    void DeviceMemory::release()
    {
        if (!m_allocated)
        {
            NARCLOG_WARNING("Device memory not allocated!");
            return;
        }

        m_allocated = false;
        vkFreeMemory(getVkDevice(), m_memory, nullptr);
    }

    void DeviceMemory::map(VkDeviceSize offset, VkDeviceSize size, void** data)
    {
        if (vkMapMemory(getVkDevice(), m_memory, offset, size, 0, data) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to map device memory!");
        }
    }

    void DeviceMemory::copy(void* dst, const void* src, VkDeviceSize size)
    {
        memcpy(dst, src, (size_t)size);
    }

    void DeviceMemory::unmap()
    {
        vkUnmapMemory(getVkDevice(), m_memory);
    }
} // namespace narc_engine