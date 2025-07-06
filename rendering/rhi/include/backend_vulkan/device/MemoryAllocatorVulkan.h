//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "device/MemoryAllocatorRhi.h"

#include "backend_vulkan/DeviceVulkan.h"

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

        NARC_OVERRIDE_GETTER(DeviceVulkan*, getDevice, super::getDevice()->getDeviceVulkan())
        NARC_OVERRIDE_GETTER(ContextVulkan*, getContext, super::getContext()->getContextVulkan())

    private:
        VmaAllocatorCreateInfo createAllocatorCreateInfo(const VmaVulkanFunctions* vulkanFunctions) const;

        static VmaVulkanFunctions createVulkanFunctions();
    };
} // narc_engine
