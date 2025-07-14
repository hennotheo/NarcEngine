//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "device/MemoryAllocatorRhi.h"

#include "DeviceVulkan.h"
#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    struct ImageResourceVulkan
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
    };

    class MemoryAllocatorVulkan final : public MemoryAllocatorRhi
    {
        using super = MemoryAllocatorRhi;

    public:
        explicit MemoryAllocatorVulkan(const ContextRhiPtr& ctx, const DeviceRhiPtr& device);
        ~MemoryAllocatorVulkan() override;

        NARC_IMPL_INITIALISABLE();

        ImageResourceVulkan createImage(const VkImageCreateInfo* pImageCreateInfo, const VmaAllocationCreateInfo* pAllocationCreateInfo) const;//TODO: Replace by builder pattern
        void destroyImage(const ImageResourceVulkan& imageResource);

    private:
        VmaAllocator m_allocator = VK_NULL_HANDLE;

        std::weak_ptr<ContextVulkan> m_context;
        std::weak_ptr<DeviceVulkan> m_device;

    private:
        VmaAllocatorCreateInfo createAllocatorCreateInfo(const VmaVulkanFunctions* vulkanFunctions) const;

        static VmaVulkanFunctions createVulkanFunctions();
    };
} // narc_engine
