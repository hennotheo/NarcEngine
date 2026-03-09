//
// Created by theo on 2/18/26.
//

#include "helpers/DeviceHelpers.h"

#include "helpers/QueueHelpers.h"
#include "helpers/SwapChainHelpers.h"
#include "surface/IVulkanSurface.h"

namespace narc_engine {

    bool isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria, const IVulkanSurface* surface) noexcept
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

        if (deviceFeatures.samplerAnisotropy == VK_FALSE)
        {
            return false;
        }

        if (!areDeviceExtensionsSupported(device, criteria.DeviceRequiredExtensions, surface))
        {
            return false;
        }

        if (surface == nullptr)
        {
            return false;
        }

        const auto vkSurface = surface->getHandle();

        if (const SwapChainSupportInfoVulkan swapChainSupport = querySwapChainSupportInfo(device, vkSurface).transform_error(
                    [](const auto& err) {
                        NARC_ERROR_RUNTIME("Surface not supported by current device");
                        return err;
                    }).value();
            swapChainSupport.Formats.empty() || swapChainSupport.PresentModes.empty())
        {
            return false;
        }

        return true;
    }

    device_score_t evaluateDeviceScore(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) noexcept
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

    bool areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                           const std::vector<VkExtensionProperties>& availableExtensions)
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

    bool areDeviceExtensionsSupported(const VkPhysicalDevice& device,
                                     const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                     const IVulkanSurface* surface) noexcept
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        const auto result = queryQueueFamilyIndices(device, surface);
        if (!result.has_value())
        {
            return false;
        }

        const auto indices = result.value();
        if (!queueFamilyIndexSupportPresentation(surface, device, indices.PresentationFamily.value_or(0)))
        {
            return false;
        }

        return areAllRequiredExtensionsAvailable(requiredExtensions, availableExtensions);
    }

    VulkanServiceQuery<VkPhysicalDevice> queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices,
                                                                  const PhysicalDeviceCriteria& criteria,
                                                                  const IVulkanSurface* surface) noexcept
    {
        if (devices.empty())
        {
            return vulkanServiceUnexpected("No physical devices to evaluate");
        }

        auto filtered = devices
                        | std::views::filter([criteria, surface](const VkPhysicalDevice& device) {
                            return isDeviceSuitable(device, criteria, surface);
                        })
                        | std::views::transform([criteria](VkPhysicalDevice device) {
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
            return vulkanServiceUnexpected("Failed to find a suitable GPU!");
        }

        return output[0].second;
    }
}
