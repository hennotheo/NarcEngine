#pragma once

#include <vulkan/vulkan_core.h>

namespace narc_engine
{
    class EngineBuilder;
    class PhysicalDevice;

    class LogicalDevice
    {
    public:
        LogicalDevice(const EngineBuilder* builder, const PhysicalDevice* physicalDevice);
        ~LogicalDevice();

        GETTER const inline VkDevice& getVkDevice() const { return m_device; }

    private:
        VkDevice m_device;

        const PhysicalDevice* m_physicalDevice = nullptr;
    };
}