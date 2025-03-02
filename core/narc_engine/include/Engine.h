#pragma once

#include <NarcIO.h>

#include "CommandPool.h"
#include "core/EngineDebugLogger.h"
#include "core/DeviceHandler.h"
#include "core/EngineBinder.h"
#include "core/EngineInstance.h"
#include "interfaces/IEngine.h"
#include "renderer/EngineRenderer.h"

namespace narc_engine {
    class Engine final : public IEngine
    {
        friend EngineBinder;

    public:
        Engine();
        ~Engine() override;

        static Engine* getInstance();

        void pollEvents() override;
        bool shouldClose() const override;
        void render() override;
        void waitDeviceIdle() override;
        EngineBinder* binder() const override;

        const DeviceHandler* getDevice() const { return &m_deviceHandler; }
        Window* getWindow() { return m_window.get(); }
        CommandPool* getCommandPool() { return &m_commandPool; }

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                         VkDeviceMemory& imageMemory) const;
        void createImage(const narc_io::Image& imageData, VkFormat format, VkImageTiling tiling,
                         VkImageUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;

    private:
        std::unique_ptr<EngineInstance> m_instance;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<EngineDebugLogger> m_debugLogger;
        DeviceHandler m_deviceHandler;

        EngineRenderer m_renderer;
        CommandPool m_commandPool;

        std::unique_ptr<EngineBinder> m_engineBinder;

    private:
        //void createVulkanInstance();
        static bool hasStencilComponent(VkFormat format);
    };
}
