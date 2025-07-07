//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "device/MemoryAllocatorRhi.h"

#include "DeviceVulkan.h"
#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    class MemoryAllocatorVulkan final : public MemoryAllocatorRhi
    {
        using super = MemoryAllocatorRhi;

    public:
        explicit MemoryAllocatorVulkan(const ContextRhiPtr& ctx, const DeviceRhiPtr& device);
        ~MemoryAllocatorVulkan() override;

        NARC_IMPL_INITIALISABLE();

    private:
        VmaAllocator m_allocator = VK_NULL_HANDLE;

        std::weak_ptr<ContextVulkan> m_context;
        std::weak_ptr<DeviceVulkan> m_device;

    private:
        VmaAllocatorCreateInfo createAllocatorCreateInfo(const VmaVulkanFunctions* vulkanFunctions) const;

        static VmaVulkanFunctions createVulkanFunctions();
    };
} // narc_engine
