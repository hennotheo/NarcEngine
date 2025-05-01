#pragma once
#include <vulkan/vulkan_core.h>

#include "core/devices/PhysicalDevice.h"
#include "core/devices/LogicalDevice.h"

class ImGui_ImplVulkan_InitInfo;

namespace narc_engine
{
    class Window;
    class ISurfaceProvider;
    class EngineBuilder;

    class DeviceHandler
    {
    public:
        explicit DeviceHandler(const EngineBuilder* builder);
        ~DeviceHandler();

        GETTER const inline PhysicalDevice* getPhysicalDevice() const { return m_physicalDevice.get(); }
        GETTER const inline LogicalDevice* getLogicalDevice() const { return m_logicalDevice.get(); }

        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;

        void waitGraphicsQueueIdle() const;
        VkResult submitGraphicsQueue(uint32_t submitCount, const VkSubmitInfo *submitInfo, VkFence fence) const;
        VkResult presentKHR(const VkPresentInfoKHR *presentInfo) const;

    private:
        std::unique_ptr<PhysicalDevice> m_physicalDevice;
        std::unique_ptr<LogicalDevice> m_logicalDevice;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;
        const KeywordList* m_deviceExtensions = nullptr;
    };
}
