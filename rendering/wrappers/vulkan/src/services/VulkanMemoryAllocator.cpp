//
// Created by theo on 12/15/25.
//

#include "services/VulkanMemoryAllocator.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "device/VulkanDevice.h"
#include "instance/VulkanInstance.h"

namespace narc_engine {
    VulkanMemoryAllocator::VulkanMemoryAllocator(const VulkanInstance* instance, const VulkanDevice* device) :
        m_instance(instance),
        m_device(device)
    {
        //Empty Constructor.
    }

    VulkanMemoryAllocator::~VulkanMemoryAllocator() = default;

    void VulkanMemoryAllocator::init()
    {
        createVmaAllocator();

        NARC_LOG_DEBUG("Vulkan Allocator created!");
    }

    void VulkanMemoryAllocator::shutdown()
    {
        destroyVmaAllocator();
        m_allocator = VK_NULL_HANDLE;

        NARC_LOG_DEBUG("Vulkan Allocator destroyed!");
    }

    VulkanServiceQuery<VmaAllocationInfo> VulkanMemoryAllocator::allocBuffer(const VkBufferCreateInfo& infos, VkBuffer& buffer,
                                                                             VmaAllocation& alloc) const noexcept
    {
        if (infos.size == 0)
        {
            return vulkanServiceUnexpected("Can't allocate 0 sized buffer.");
        }

        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                               VMA_ALLOCATION_CREATE_MAPPED_BIT;
        allocationInfo.pUserData = const_cast<void*>(static_cast<const void*>(this));
        
        VmaAllocationInfo stagingInfo;
        if (vmaCreateBuffer(m_allocator, &infos, &allocationInfo, &buffer, &alloc, &stagingInfo)!= VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create buffer.");
        }

        return stagingInfo;
    }

    bool VulkanMemoryAllocator::deallocBuffer(VkBuffer& buffer, VmaAllocation& alloc) const noexcept
    {
        if (alloc == VK_NULL_HANDLE)
        {
            return false;
        }
        
        vmaDestroyBuffer(m_allocator, buffer, alloc);

        return true;
    }

    VulkanServiceQuery<VmaAllocationInfo> VulkanMemoryAllocator::allocImage(const VkImageCreateInfo& infos, VkImage& buffer,
                                                                            VmaAllocation& alloc) const noexcept
    {
        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
        // allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
        //                        VMA_ALLOCATION_CREATE_MAPPED_BIT |
        //                            VMA_ALLOCATION_CREATE_;
        // For very big textures (>128MB for example)
        // allocationInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
        allocationInfo.pUserData = (void*)this;
        
        VmaAllocationInfo stagingInfo;
        if (vmaCreateImage(m_allocator, &infos, &allocationInfo, &buffer, &alloc, &stagingInfo)!= VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create image.");
        }

        return stagingInfo;
    }

    bool VulkanMemoryAllocator::deallocImage(VkImage& image, VmaAllocation& alloc) const noexcept
    {
        if (alloc == VK_NULL_HANDLE)
        {
            return false;
        }

        vmaDestroyImage(m_allocator, image, alloc);

        return true;
    }

    VulkanServiceQuery<VkImageView> VulkanMemoryAllocator::allocImageView(const VkImageViewCreateInfo& infos) const noexcept
    {
        VkImageView imageView = VK_NULL_HANDLE;
        if (vkCreateImageView(m_device->getHandle(), &infos, nullptr, &imageView) != VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create image view.");
        }

        return imageView;
    }

    bool VulkanMemoryAllocator::deallocImageView(const VkImageView& imageView) const noexcept
    {
        vkDestroyImageView(m_device->getHandle(), imageView, nullptr);

        return true;
    }

    VulkanServiceQuery<VkSampler> VulkanMemoryAllocator::allocSampler(VkSamplerCreateInfo& infos) const noexcept
    {
        VkSampler sampler = VK_NULL_HANDLE;

        infos.maxAnisotropy = m_device->getPhysicalDeviceProperties().limits.maxSamplerAnisotropy;

        if (vkCreateSampler(m_device->getHandle(), &infos, nullptr, &sampler) != VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create sampler.");
        }

        return sampler;
    }

    bool VulkanMemoryAllocator::deallocSampler(const VkSampler& sampler) const noexcept
    {
        vkDestroySampler(m_device->getHandle(), sampler, nullptr);

        return true;
    }

    narc_core::result VulkanMemoryAllocator::mapMemory(const Memory* data, const VkDeviceSize& dataSize, const VmaAllocation& alloc) const noexcept
    {
        if (!m_allocator || !data || dataSize == 0 || alloc == VK_NULL_HANDLE)
        {
            return false;
        }

        void* ptr = nullptr;
        VkResult result = vmaMapMemory(m_allocator, alloc, &ptr);
        if (result != VK_SUCCESS || !ptr)
        {
            return false;
        }

        memcpy(ptr, data, dataSize);
        vmaFlushAllocation(m_allocator, alloc, 0, dataSize);
        vmaUnmapMemory(m_allocator, alloc);

        return true;
    }

    void VulkanMemoryAllocator::createVmaAllocator()
    {
        VmaAllocatorCreateInfo allocCI{};
        allocCI.instance = m_instance->getHandled();
        allocCI.physicalDevice = m_device->getPhysicalDeviceHandle();
        allocCI.device = m_device->getHandle();

        vmaCreateAllocator(&allocCI, &m_allocator);
    }

    void VulkanMemoryAllocator::destroyVmaAllocator()
    {
        vmaDestroyAllocator(m_allocator);
    }
} // narc_engine
