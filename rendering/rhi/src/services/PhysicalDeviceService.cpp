//
// Created by theohenno on 10/15/25.
//

#include "services/PhysicalDeviceService.h"

#include "../../include/vulkan_wrappers/VulkanInstance.h"
#include "VulkanSurfacesManager.h"
#include "services/SwapChainService.h"

#include "models/SwapChainSupportInfoVulkan.h"

namespace narc_engine {

    PhysicalDeviceService::PhysicalDeviceService(std::weak_ptr<VulkanInstance> instance,
                                                 const std::shared_ptr<SwapChainService>& swapChainService,
                                                 std::weak_ptr<VulkanSurfacesManager> surfacesManager) :
        m_instance(std::move(instance)),
        m_surfacesManager(std::move(surfacesManager)),
        m_swapChainService(swapChainService)
    {
    }

    PhysicalDeviceService::~PhysicalDeviceService() = default;

    QUERY(std::vector<VkPhysicalDevice>, QueryDeviceError) PhysicalDeviceService::queryAllPhysicalDevices() const noexcept
    {
        NARC_GUARD_WEAK_UNEXPECTED(instance, m_instance, "Failed to create PhysicalDeviceService");

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance->getHandled(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            std::unexpected(QueryDeviceError{"Failed to find GPUs with Vulkan support!"});
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance->getHandled(), &deviceCount, devices.data());

        return devices;
    }

    QUERY(VkPhysicalDevice, QueryDeviceError) PhysicalDeviceService::queryBestPhysicalDevices(
            std::vector<VkPhysicalDevice> devices, const PhysicalDeviceCriteria& criteria) const noexcept
    {
        if (devices.empty())
        {
            return std::unexpected(QueryDeviceError{"No physical devices to evaluate"});
        }

        auto filtered = devices
                        | std::views::filter([this, criteria](const VkPhysicalDevice& device) {
                            return isDeviceSuitable(device, criteria);
                        })
                        | std::views::transform([this, criteria](VkPhysicalDevice device) {
                            return std::pair{
                                    evaluateDeviceScore(device, criteria), device
                            };
                        });

        std::vector<std::pair<device_score_t, VkPhysicalDevice>> output;
        output.reserve(devices.size());
        std::ranges::sort(output, [](auto const& a, auto const& b) {
            return a.first > b.first;
        });
        std::ranges::copy(filtered, std::back_inserter(output));

        if (output.empty())
        {
            return std::unexpected(QueryDeviceError{"Failed to find a suitable GPU!"});
        }

        return output[0].second;
    }

    bool PhysicalDeviceService::isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if (criteria.RequireGeometryShader && deviceFeatures.geometryShader == VK_FALSE)
        {
            return false;
        }

        if (criteria.RequireDiscreteGPU && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            return false;
        }

        if (!areDeviceExtensionSupported(device, criteria.DeviceRequiredExtensions))
        {
            return false;
        }

        {
            const auto surfaceManager = m_surfacesManager.lock();
            if (!surfaceManager)
            {
                return false;
            }

            const auto surface = surfaceManager->getMainSurface()->getHandled();
            if (const SwapChainSupportInfoVulkan swapChainSupport = m_swapChainService->querySwapChainSupportInfo(device, surface);
                swapChainSupport.Formats.empty() || swapChainSupport.PresentModes.empty())
            {
                return false;
            }
        }

        return true;
    }

    PhysicalDeviceService::device_score_t PhysicalDeviceService::evaluateDeviceScore(const VkPhysicalDevice& device,
                                                                                     const PhysicalDeviceCriteria& criteria) const noexcept
    {
        device_score_t score = 0;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if (criteria.PreferDiscreteGPU && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        return score;
    }

    bool PhysicalDeviceService::areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                                                  const std::vector<VkExtensionProperties>& availableExtensions) const
    {
        std::set<std::string> remaining;
        for (const auto& extension: requiredExtensions)
        {
            for (const auto& name: extension->getExtensionNames())
            {
                remaining.insert(name);
            }
        }

        for (const auto& extension: availableExtensions)
        {
            remaining.erase(extension.extensionName);
            if (remaining.empty())
            {
                return true;
            }
        }

        return remaining.empty();
    }

    bool PhysicalDeviceService::areDeviceExtensionSupported(const VkPhysicalDevice& device,
                                                            const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions) const noexcept
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        return areAllRequiredExtensionsAvailable(requiredExtensions, availableExtensions);
    }
} // narc_engine
