//
// Created by theohenno on 10/14/25.
//

#include "device/VulkanDevice.h"

#include "VulkanTextureImage.h"
#include "buffers/VulkanBuffer.h"
#include "buffers/VulkanIndexBuffer.h"
#include "buffers/VulkanStagingBuffer.h"
#include "buffers/VulkanUniformBuffer.h"
#include "buffers/VulkanVertexBuffer.h"
#include "instance/VulkanInstance.h"
#include "sync/VulkanFence.h"

#include "helpers/DeviceHelpers.h"
#include "helpers/QueueHelpers.h"
#include "mapping/mappingToVk.h"

namespace narc_engine {
    VulkanDevice::VulkanDevice(const VulkanInstance* instance) :
        m_instance(instance),
        m_memoryAllocator(instance, this)
    {

    }

    VulkanDevice::~VulkanDevice() = default;

    void VulkanDevice::init()
    {
        selectPhysicalDeviceFromCriteria();
        selectQueueFamily();

        createDevice();

        fillQueues(m_queueFamilyIndices);
        m_graphicsQueue.init();
        m_presentQueue.init();

        m_memoryAllocator.init();

        NARC_LOG_DEBUG("Vulkan Device created successfully!");
    }

    void VulkanDevice::shutdown()
    {
        m_memoryAllocator.shutdown();

        m_presentQueue.shutdown();
        m_graphicsQueue.shutdown();

        vkDestroyDevice(m_device, nullptr);

        m_device = VK_NULL_HANDLE;
        m_physicalDevice = VK_NULL_HANDLE;
    }

    RhiQuery<std::unique_ptr<IBuffer>> VulkanDevice::createBuffer(const BufferAllocationInfo infos) const noexcept
    {
        if (infos.IsVertexBuffer)
        {
            return std::make_unique<VulkanVertexBuffer>(&m_memoryAllocator, infos.Size);
        }

        if (infos.IsIndexBuffer)
        {
            return std::make_unique<VulkanIndexBuffer>(&m_memoryAllocator, infos.Size);
        }

        if (infos.IsStaging)
        {
            return std::make_unique<VulkanStagingBuffer>(&m_memoryAllocator, infos.Size);
        }

        return std::make_unique<VulkanUniformBuffer>(&m_memoryAllocator, infos.Size);
    }

    RhiQuery<std::unique_ptr<IImage>> VulkanDevice::createImage(const ImageAllocationInfo& infos) const noexcept
    {
        auto texture = std::make_unique<VulkanTextureImage>(&m_memoryAllocator);
        texture->setExtent(infos.Extent);

        return texture;
    }

    narc_core::result VulkanDevice::waitForFences(const std::span<const IFence*> fences) const
    {
        const auto vkFences = mapping::toVkFenceArray(fences);

        if (vkWaitForFences(m_device, vkFences.size(), vkFences.data(), VK_TRUE, UINT64_MAX) != VK_SUCCESS)
        {
            NARC_LOG_ERROR("Waiting for fences has failed.");
            return false;
        }

        return true;
    }

    narc_core::result VulkanDevice::resetFences(const std::span<const IFence*> fences) const
    {
        const auto vkFences = mapping::toVkFenceArray(fences);

        if (vkResetFences(m_device, vkFences.size(), vkFences.data()) != VK_SUCCESS)
        {
            NARC_LOG_ERROR("Resetting fences has failed.");
            return false;
        }

        return true;
    }

    void VulkanDevice::waitIdle() const
    {
        vkDeviceWaitIdle(m_device);
    }

    void VulkanDevice::selectPhysicalDeviceFromCriteria()
    {
        const auto devices = m_instance->queryAllPhysicalDevices();
        if (!devices.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        const auto bestDeviceResult = queryBestPhysicalDevices(devices.value(), m_physicalDeviceCriteria, m_mainWindowSurface);
        if (!bestDeviceResult.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        m_physicalDevice = bestDeviceResult.value();
        vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
    }

    void VulkanDevice::selectQueueFamily()
    {
        const auto queueFamilyIndicesResult = queryQueueFamilyIndices(m_physicalDevice, m_mainWindowSurface);
        if (!queueFamilyIndicesResult.has_value())
        {
            NARC_ERROR_RUNTIME("Failed to find required queue families.");
        }

        if (!queueFamilyIndicesResult->isComplete())
        {
            NARC_LOG_WARNING("Queue family indices is not complete.");
        }

        m_queueFamilyIndices = queueFamilyIndicesResult.value();
    }

    void VulkanDevice::createDevice()
    {
        const auto uniqueQueueFamilies = getUniqueIndices(m_queueFamilyIndices).transform_error(
                [](const auto& err) {
                    NARC_ERROR_RUNTIME("Uniques queues not supported by current device");
                    return err;
                }).value();

        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(uniqueQueueFamilies.size());
        for (const uint32_t family: uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = family;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.enabledExtensionCount = deviceExtensions.size();
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.enabledLayerCount = 0;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create logical device!");
        }
    }

    void VulkanDevice::fillQueues(const QueueFamilyIndices& queueFamilyIndices)
    {
        m_presentQueue.setDevice(this);
        m_graphicsQueue.setDevice(this);

        m_presentQueue.setQueueIndex(0); //Cf vulkan doc
        m_graphicsQueue.setQueueIndex(0);

        m_presentQueue.setQueueFamilyIndex(queueFamilyIndices.PresentationFamily.value_or(QUEUE_INDEX_NONE));
        m_graphicsQueue.setQueueFamilyIndex(queueFamilyIndices.GraphicsFamily.value_or(QUEUE_INDEX_NONE));
    }
} // namespace narc_engine
