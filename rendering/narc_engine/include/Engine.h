#pragma once

#include "interfaces/IEngine.h"
#include "core/interfaces/IEngineCallbacks.h"

#include "CommandPool.h"

#include "core/DeviceHandler.h"
#include "core/EngineBinder.h"
#include "core/EngineDebugLogger.h"
#include "core/EngineInstance.h"
#include "core/interfaces/ISurfaceProvider.h"

#include "renderer/EngineRenderer.h"

namespace narc_engine {

    class Engine final : public IEngine, public IEngineCallbacks
    {
        friend EngineBinder;

    public:
        Engine();
        ~Engine() override;

        static Engine* getInstance();

        GETTER bool shouldClose() const override { return m_shouldClose; }

        void stop() override { m_shouldClose = true; }
        void pollEvents() override;
        void render() override;
        void waitDeviceIdle() override;
        EngineBinder* binder() const override;
        EngineResourcesManager* resourceManager() const override;

        GETTER const DeviceHandler* getDevice() const { return m_deviceHandler.get(); }

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
        std::unique_ptr<ISurfaceProvider> m_surfaceProvider;
        std::unique_ptr<EngineDebugLogger> m_debugLogger;
        std::unique_ptr<DeviceHandler> m_deviceHandler;

        std::unique_ptr<CommandPool> m_commandPool;
        std::unique_ptr<EngineRenderer> m_renderer;

        std::unique_ptr<EngineBinder> m_engineBinder;
        std::unique_ptr<EngineResourcesManager> m_resourcesManager;

        bool m_shouldClose = false;

    private:
        GETTER CommandPool* getCommandPool() const { return m_commandPool.get(); }

        static bool hasStencilComponent(VkFormat format);
    };
}
