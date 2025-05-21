#pragma once

#include "core/EngineBuilder.h"

#include "interfaces/IEngine.h"

#include "platform/vulkan/CommandPool.h"

#include "core/devices/DeviceHandler.h"
#include "core/EngineBinder.h"
#include "core/EngineDebugLogger.h"
#include "core/EngineInstance.h"
#include "core/interfaces/ISurfaceProvider.h"

#include "core/queues/GraphicsQueue.h"
#include "core/queues/PresentQueue.h"

#include "resources/ResourceManager.h"

namespace narc_engine {

    class DeviceMemory;

    class Engine : public IEngine
    {
        friend EngineBinder;

    public:
        Engine();
        ~Engine() override;

        static Engine* getInstance();

        GETTER bool shouldClose() const override { return m_shouldClose; }

        void pollEvents() override;
        void render() override;
        void waitDeviceIdle() override;
        EngineBinder* binder() const override;
        ResourceManager* resourceManager() const { return m_resourcesManager.get(); }
        
        GETTER CommandPool* getCommandPool() const { return m_commandPool.get(); }
        TEMP_CODE GETTER const DeviceHandler* getDevice() const { return m_deviceHandler.get(); }
        TEMP_CODE GETTER const GraphicsQueue* getGraphicsQueue() const { return m_graphicsQueue.get(); }
        TEMP_CODE GETTER const PresentQueue* getPresentQueue() const { return m_presentQueue.get(); }
        TEMP_CODE GETTER const EngineInstance* getEngineInstance() const { return m_instance.get(); }

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
            DeviceMemory* imageMemory) const;
        void createImage(const narc_io::Image& imageData, VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties, VkImage& image, DeviceMemory* imageMemory) const;

    private:
        ContextRhiPtr m_contextRhi;
        std::unique_ptr<EngineInstance> m_instance;
        std::unique_ptr<EngineDebugLogger> m_debugLogger;
        std::unique_ptr<DeviceHandler> m_deviceHandler;

        std::unique_ptr<Window> m_windows;//TODO enhance to multiwindow

        std::unique_ptr<GraphicsQueue> m_graphicsQueue;
        std::unique_ptr<PresentQueue> m_presentQueue;

        std::unique_ptr<CommandPool> m_commandPool;

        std::unique_ptr<EngineBinder> m_engineBinder;
        std::unique_ptr<ResourceManager> m_resourcesManager;

        bool m_shouldClose = false;

    private:
        static bool hasStencilComponent(VkFormat format);
    };
}
