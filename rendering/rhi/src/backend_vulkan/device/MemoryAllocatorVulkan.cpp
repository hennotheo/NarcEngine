//
// Created by theoh on 7/6/2025.
//

#include "backend_vulkan/device/MemoryAllocatorVulkan.h"

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0 // NOSONAR
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1 // NOSONAR
#include <vk_mem_alloc.h>

#include "../../../include/backend_vulkan/device/DeviceVulkan.h"

namespace narc_engine
{
    MemoryAllocatorVulkan::MemoryAllocatorVulkan(const ContextRhiPtr& ctx, const DeviceRhiPtr& device) :
        super(ctx, device)
    {
    }

    MemoryAllocatorVulkan::~MemoryAllocatorVulkan() = default;

    void MemoryAllocatorVulkan::init()
    {
        const auto vulkanFunctions = createVulkanFunctions();
        const VmaAllocatorCreateInfo allocatorCreateInfo = createAllocatorCreateInfo(&vulkanFunctions);

        vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);
    }

    void MemoryAllocatorVulkan::shutdown()
    {
        vmaDestroyAllocator(m_allocator);
    }

    VmaAllocatorCreateInfo MemoryAllocatorVulkan::createAllocatorCreateInfo(const VmaVulkanFunctions* vulkanFunctions) const
    {
        const ApplicationInfos appInfo = getContext()->getApplicationInfos();

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocatorCreateInfo.vulkanApiVersion = appInfo.RendererApiVersion;
        allocatorCreateInfo.physicalDevice = getDevice()->getPhysicalDeviceProperties().PhysicalDevice;
        allocatorCreateInfo.device = getDevice()->getVkDevice();
        allocatorCreateInfo.instance = getContext()->getVkInstance();
        allocatorCreateInfo.pVulkanFunctions = vulkanFunctions;

        return allocatorCreateInfo;
    }

    VmaVulkanFunctions MemoryAllocatorVulkan::createVulkanFunctions()
    {
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

        return vulkanFunctions;
    }
} // narc_engine
