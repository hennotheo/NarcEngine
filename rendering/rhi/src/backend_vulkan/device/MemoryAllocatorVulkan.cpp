//
// Created by theoh on 7/6/2025.
//

#include "backend_vulkan/device/MemoryAllocatorVulkan.h"

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0 // NOSONAR
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1 // NOSONAR
#include <vk_mem_alloc.h>

#include "backend_vulkan/device/DeviceVulkan.h"

namespace narc_engine
{
    MemoryAllocatorVulkan::MemoryAllocatorVulkan(const ContextRhiPtr& ctx, const DeviceRhiPtr& device) :
        m_context(std::static_pointer_cast<ContextVulkan>(ctx)), // Static cast cause ContextRhiPtr is a shared pointer to ContextVulkan
        m_device(std::static_pointer_cast<DeviceVulkan>(device)) // Static cast cause DeviceRhiPtr is a shared pointer to DeviceVulkan
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

    ImageResourceVulkan MemoryAllocatorVulkan::createImage(const VkImageCreateInfo* pImageCreateInfo,
                                                           const VmaAllocationCreateInfo* pAllocationCreateInfo) const
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation = VK_NULL_HANDLE;
        vmaCreateImage(m_allocator, pImageCreateInfo, pAllocationCreateInfo, &image, &allocation, nullptr);

        return ImageResourceVulkan{
            .image = image,
            .allocation = allocation
        };
    }

    void MemoryAllocatorVulkan::destroyImage(const ImageResourceVulkan& imageResource)
    {
        vmaCreateImage(m_allocator, &desc.imageInfo, &desc.allocInfo, &res.image, &res.allocation, nullptr);
    }

    VmaAllocatorCreateInfo MemoryAllocatorVulkan::createAllocatorCreateInfo(const VmaVulkanFunctions* vulkanFunctions) const
    {
        NARC_GUARD_WEAK(context, m_context, "Context is null!");
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        const ApplicationInfos appInfo = context->getApplicationInfos();

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocatorCreateInfo.vulkanApiVersion = appInfo.RendererApiVersion;
        allocatorCreateInfo.physicalDevice = device->getPhysicalDeviceProperties().PhysicalDevice;
        allocatorCreateInfo.device = device->getVkDevice();
        allocatorCreateInfo.instance = context->getVkInstance();
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
