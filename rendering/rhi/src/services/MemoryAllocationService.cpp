//
// Created by theo on 12/15/25.
//

#include "services/MemoryAllocationService.h"

#include "vulkan_wrappers/VulkanDevice.h"
#include "vulkan_wrappers/VulkanInstance.h"

namespace narc_engine {
    MemoryAllocationService::MemoryAllocationService(
            NARC_DI_IMPORT_SERVICE(VulkanInstance),
            NARC_DI_IMPORT_SERVICE(VulkanDevice)) :
        NARC_DI_IMPL_SERVICE(VulkanInstance, m_instance),
        NARC_DI_IMPL_SERVICE(VulkanDevice, m_device)
    {
    }

    MemoryAllocationService::~MemoryAllocationService() = default;

    VulkanServiceQuery<VmaAllocationInfo> MemoryAllocationService::allocBuffer(const VkBufferCreateInfo& infos, VkBuffer& buffer, VmaAllocation& alloc) const noexcept
    {
        if (infos.size == 0)
        {
            return vulkanServiceUnexpected("Can't allocate 0 sized buffer.");
        }
        
        checkBeforeAllocItem();

        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocationInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                               VMA_ALLOCATION_CREATE_MAPPED_BIT;
        allocationInfo.pUserData = (void*)this;
        
        VmaAllocationInfo stagingInfo;
        if (vmaCreateBuffer(m_allocator, &infos, &allocationInfo, &buffer, &alloc, &stagingInfo)!= VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create buffer.");
        }
        
        return stagingInfo;
    }
    
    bool MemoryAllocationService::deallocBuffer(VkBuffer& buffer, VmaAllocation& alloc) const noexcept
    {
        if (alloc == VK_NULL_HANDLE)
        {
            return false;
        }
        
        vmaDestroyBuffer(m_allocator, buffer, alloc);
        
        checkAfterDeallocItem();
        
        return true;
    }

    VulkanServiceQuery<VmaAllocationInfo> MemoryAllocationService::allocImage(const VkImageCreateInfo& infos, VkImage& buffer,
            VmaAllocation& alloc) const noexcept
    {        
        checkBeforeAllocItem();

        VmaAllocationCreateInfo allocationInfo{};
        allocationInfo.usage = VMA_MEMORY_USAGE_AUTO;
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

    bool MemoryAllocationService::deallocImage(VkImage& image, VmaAllocation& alloc) const noexcept
    {
        if (alloc == VK_NULL_HANDLE)
        {
            return false;
        }
        
        vmaDestroyImage(m_allocator, image, alloc);
        
        checkAfterDeallocItem();
        
        return true;
    }

    VulkanServiceQuery<VkImageView> MemoryAllocationService::allocImageView(const VkImageViewCreateInfo& infos) const noexcept
    {
        VkImageView imageView = VK_NULL_HANDLE;
        if (vkCreateImageView(m_device->getHandle(), &infos, nullptr, &imageView) != VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create image view.");
        }

        return imageView;
    }

    bool MemoryAllocationService::deallocImageView(const VkImageView& imageView) const noexcept
    {
        vkDestroyImageView(m_device->getHandle(), imageView, nullptr);

        return true;
    }

    VulkanServiceQuery<VkSampler> MemoryAllocationService::allocSampler(VkSamplerCreateInfo& infos) const noexcept
    {
        VkSampler sampler = VK_NULL_HANDLE;

        infos.maxAnisotropy = m_device->getPhysicalDeviceProperties().limits.maxSamplerAnisotropy;

        if (vkCreateSampler(m_device->getHandle(), &infos, nullptr, &sampler) != VK_SUCCESS)
        {
            return vulkanServiceUnexpected("Can't create sampler.");
        }

        return sampler;
    }

    bool MemoryAllocationService::deallocSampler(const VkSampler& sampler) const noexcept
    {
        vkDestroySampler(m_device->getHandle(), sampler, nullptr);

        return true;
    }

    narc_core::result MemoryAllocationService::mapMemory(const void* data, const VkDeviceSize& dataSize, const VmaAllocation& alloc) const noexcept
    {
        if (!m_allocator || !data || dataSize == 0 || alloc == VK_NULL_HANDLE)
        {
            return false;
        }

        void* ptr = nullptr;
        VkResult r = vmaMapMemory(m_allocator, alloc, &ptr);
        if (r != VK_SUCCESS || !ptr)
            return false;

        memcpy(ptr, data, dataSize);
        vmaFlushAllocation(m_allocator, alloc, 0, VK_WHOLE_SIZE);
        vmaUnmapMemory(m_allocator, alloc);
        return true;
    }

    void MemoryAllocationService::checkBeforeAllocItem() const
    {
        if (m_allocatedItemCount == 0)
        {
            createVmaAllocator();
        }
        
        m_allocatedItemCount++;
    }

    void MemoryAllocationService::checkAfterDeallocItem() const
    {
        m_allocatedItemCount--;
        
        if (m_allocatedItemCount == 0)
        {
            destroyVmaAllocator();
        }
    }

    void MemoryAllocationService::createVmaAllocator() const
    {
        VmaAllocatorCreateInfo allocCI{};
        allocCI.instance = m_instance->getHandled();
        allocCI.physicalDevice = m_device->getPhysicalDeviceHandle();
        allocCI.device = m_device->getHandle();

        vmaCreateAllocator(&allocCI, &m_allocator);
    }

    void MemoryAllocationService::destroyVmaAllocator() const
    {
        vmaDestroyAllocator(m_allocator);
    }
} // narc_engine
