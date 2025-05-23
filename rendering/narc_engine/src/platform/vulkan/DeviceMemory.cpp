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
            return;
        }

        m_allocated = true;
        if (vkAllocateMemory(NARC_DEVICE_HANDLE, &m_allocInfo, nullptr, &m_memory) != VK_SUCCESS)
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
        vkFreeMemory(NARC_DEVICE_HANDLE, m_memory, nullptr);
    }

    void DeviceMemory::map(VkDeviceSize offset, VkDeviceSize size, void** data)
    {
        if (vkMapMemory(NARC_DEVICE_HANDLE, m_memory, offset, size, 0, data) != VK_SUCCESS)
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
        vkUnmapMemory(NARC_DEVICE_HANDLE, m_memory);
    }
} // namespace narc_engine