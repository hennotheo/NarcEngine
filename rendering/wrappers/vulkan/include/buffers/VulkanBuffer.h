//
// Created by theo on 3/1/26.
//

#pragma once

namespace narc_engine {
    class VulkanBuffer : public IBuffer
    {
        friend class VulkanDevice;
    public:
        VulkanBuffer();
        VulkanBuffer(const VulkanBuffer&) = delete;
        ~VulkanBuffer() override = default;

    private:
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };
} // narc_engine
