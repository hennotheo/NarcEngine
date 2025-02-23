#pragma once

#include <vulkan/vulkan.h>
#include <NarcIO.h>

#include "CommandPool.h"
#include "renderer/DescriptorPool.h"
#include "core/Window.h"
#include "core/EngineDebugLogger.h"
#include "core/DeviceHandler.h"
#include "renderer/EngineRenderer.h"
#include "buffers/UniformBuffer.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine {
    struct Vertex;
    struct Image;

    class Engine
    {
    public:
        Engine();
        ~Engine();

        static Engine* getInstance();

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
        CommandPool m_commandPool;
    private:
        void createVulkanInstance();
    };
}
