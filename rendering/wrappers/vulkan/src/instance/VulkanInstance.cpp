//
// Created by theohenno on 10/11/25.
//

#include "instance/VulkanInstance.h"

#include "mapping/mappingToVk.h"

namespace narc_engine {
    VulkanInstance::VulkanInstance() = default;

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        std::vector<const char*> layerNames = {
                "VK_LAYER_KHRONOS_validation"
        };

        m_appInfo = mapping::mapFromApplicationInfo(m_applicationInfo);
        createVkInstance(m_appInfo, m_extensions, layerNames);

        NARC_LOG_DEBUG("Vulkan Instance created successfully for application: {}", m_applicationInfo.ApplicationName.c_str());

        //Init Extensions
    }

    void VulkanInstance::shutdown()
    {
        //Shutdown Extensions

        destroyVkInstance();
        m_extensions.clear();

        NARC_LOG_DEBUG("Vulkan Instance destroyed successfully");
    }

    void VulkanInstance::addExtension(std::unique_ptr<IVulkanExtension> extension)
    {
        m_extensions.push_back(std::move(extension));
    }

    VulkanServiceQuery<std::vector<VkPhysicalDevice>> VulkanInstance::queryAllPhysicalDevices() const noexcept
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            return vulkanServiceUnexpected("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

        return devices;
    }

    void VulkanInstance::createVkInstance(const VkApplicationInfo& appInfos,
                                          const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                                          const std::span<const char*>& layerNames)
    {
        std::vector<const char*> deviceExtensionNames{
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_XCB_SURFACE_EXTENSION_NAME
        };
        deviceExtensionNames.reserve(extensions.size());
        for (const auto& ext: extensions)
        {
            for (const auto names = ext->getExtensionNames();
                 const char* name: names)
            {
                deviceExtensionNames.push_back(name);
            }
        }

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfos;
        createInfo.enabledLayerCount = layerNames.size();
        createInfo.ppEnabledLayerNames = layerNames.data();
        createInfo.enabledExtensionCount = deviceExtensionNames.size();
        createInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
        if (!extensions.empty())
        {
            createInfo.pNext = extensions[0]->getCreationInfos();
        }

        if (const VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &m_instance);
            creationResult != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create Vulkan instance");
        }
    }

    void VulkanInstance::destroyVkInstance()
    {
        vkDestroyInstance(m_instance, nullptr);
    }
} // namespace narc_engine
