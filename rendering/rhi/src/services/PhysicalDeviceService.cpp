//
// Created by theohenno on 10/15/25.
//

#include "services/PhysicalDeviceService.h"

#include "models/SwapChainSupportInfoVulkan.h"
#include "models/PhysicalDeviceCriteria.h"

#include "IVulkanSurface.h"
#include "vulkan_wrappers/VulkanInstance.h"

namespace narc_engine {
    DeviceService::DeviceService(NARC_DI_IMPORT_COMPONENT(VulkanInstance),
                                 NARC_DI_IMPORT_SERVICE(ISwapchainService),
                                 NARC_DI_IMPORT_COMPONENT(ISurfacesHandler)) :
        NARC_DI_IMPL_SERVICE(VulkanInstance, m_instance),
        NARC_DI_IMPL_SERVICE(ISurfacesHandler, m_surfacesManager),
        NARC_DI_IMPL_SERVICE(ISwapchainService, m_swapChainService)
    {
    }

    DeviceService::~DeviceService() = default;

    VulkanServiceQuery<std::vector<VkPhysicalDevice>> DeviceService::queryAllPhysicalDevices() const noexcept
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance->getHandled(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            return vulkanServiceUnexpected("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance->getHandled(), &deviceCount, devices.data());

        return devices;
    }

    VulkanServiceQuery<VkPhysicalDevice> DeviceService::queryBestPhysicalDevices(
            std::vector<VkPhysicalDevice> devices, const PhysicalDeviceCriteria& criteria) const noexcept
    {
        if (devices.empty())
        {
            return vulkanServiceUnexpected("No physical devices to evaluate");
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
            return vulkanServiceUnexpected("Failed to find a suitable GPU!");
        }

        return output[0].second;
    }

    VulkanServiceQuery<VkPhysicalDeviceProperties> DeviceService::queryDeviceProperties(VkPhysicalDevice device) const noexcept
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);

        return props;
    }

    bool DeviceService::isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept
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

        if (!areDeviceExtensionSupported(device, criteria.DeviceRequiredExtensions))
        {
            return false;
        }

        const auto surface = m_surfacesManager->getMainSurface();
        if (surface == nullptr)//TODO: Handle multiple surfaces
        {
            return false;
        }
        const auto vkSurface = surface->getHandled();
        
        if (const SwapChainSupportInfoVulkan swapChainSupport = m_swapChainService->querySwapChainSupportInfo(device, vkSurface).transform_error(
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

    DeviceService::device_score_t DeviceService::evaluateDeviceScore(const VkPhysicalDevice& device,
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

    bool DeviceService::areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
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

    bool DeviceService::areDeviceExtensionSupported(const VkPhysicalDevice& device,
                                                    const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions) const noexcept
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        return areAllRequiredExtensionsAvailable(requiredExtensions, availableExtensions);
    }
} // narc_engine
