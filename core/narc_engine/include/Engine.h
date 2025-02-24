#pragma once

#include <vulkan/vulkan.h>
#include <NarcIO.h>

#include "CommandPool.h"
#include "core/Window.h"
#include "core/EngineDebugLogger.h"
#include "core/DeviceHandler.h"
#include "core/EngineBinder.h"
#include "renderer/EngineRenderer.h"

namespace narc_engine
{
    class Engine
    {
        friend class EngineBinder;
    public:
        Engine();
        ~Engine();

        static Engine* getInstance();
        static EngineBinder* binder();

        const DeviceHandler* getDevice() const { return &m_deviceHandler; }
        Window* getWindow() { return &m_window; }
        CommandPool* getCommandPool() { return &m_commandPool; }

        void pollEvents() { m_window.update(); }
        bool shouldClose() const { return m_window.shouldClose(); }
        void render() { m_renderer.drawFrame(); }

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void createImage(const narc_io::Image& imageData, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;

    private:
        Window m_window;
        EngineDebugLogger m_debugLogger;
        VkInstance m_vulkanInstance;
        DeviceHandler m_deviceHandler;

        EngineRenderer m_renderer;
        std::unique_ptr<EngineBinder> m_engineBinder;
        CommandPool m_commandPool;

    private:
        void createVulkanInstance();
    };
}
