//
// Created by theohenno on 12/3/25.
//

#include "services/InstanceService.h"

namespace narc_engine {
    VkApplicationInfo InstanceService::mapApplicationInfoToVkApplicationInfo(const ApplicationInfo& value) const noexcept
    {
        return VkApplicationInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = value.ApplicationName.c_str(),
                .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
                .pEngineName = value.EngineName.c_str(),
                .engineVersion = VK_MAKE_VERSION(0, 1, 0),
                .apiVersion = VK_API_VERSION_1_3
        };
    }

    VulkanServiceQuery<VkInstance> InstanceService::createVkInstance(const VkApplicationInfo& appInfos,
                                                                      const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                                                                      const std::span<const char*>& layerNames) const noexcept
    {
        std::vector<const char*> deviceExtensionNames{};
        deviceExtensionNames.reserve(extensions.size());
        for (const auto& ext: extensions)
        {
            for (const auto names = ext->getExtensionNames();
                 const char* name: names)
            {
                deviceExtensionNames.push_back(name);
            }
        }

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfos;
        createInfo.enabledLayerCount = layerNames.size();
        createInfo.ppEnabledLayerNames = layerNames.data();
        createInfo.enabledExtensionCount = deviceExtensionNames.size();
        createInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
        createInfo.pNext = extensions[0]->getCreationInfos();

        VkInstance instance = VK_NULL_HANDLE;
        if (const VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &instance);
            creationResult != VK_SUCCESS)
        {
            return std::unexpected("Can't create Vulkan Instance.");
        }

        return instance;
    }

    void InstanceService::destroyVkInstance(const VkInstance& instance) const noexcept
    {
        vkDestroyInstance(instance, nullptr);
    }
} // narc_engine
