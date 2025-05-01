#pragma once

#include <vulkan/vulkan_core.h>

namespace narc_engine
{
    class EngineBuilder;
    class PhysicalDevice;
    struct QueueFamilyIndices;

    class LogicalDevice
    {
    public:
        LogicalDevice(const EngineBuilder* builder, const PhysicalDevice* physicalDevice);
        ~LogicalDevice();

        GETTER const VkDevice& getVkDevice() const { return m_device; }

        void waitDeviceIdle() const;

    private:
        VkDevice m_device;

        const PhysicalDevice* m_physicalDevice = nullptr;

    private:
        std::vector<VkDeviceQueueCreateInfo> createQueueCreateInfos(const QueueFamilyIndices& indices);
    };
}