#include "platform/vulkan/DeviceMemory.h"

namespace narc_engine
{
    DeviceMemory::DeviceMemory()
    {
        m_memory = VK_NULL_HANDLE;
        m_allocInfo = {};
        m_allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    }

    DeviceMemory::~DeviceMemory()
    {
        if(m_allocated)
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
} // namespace narc_engine
