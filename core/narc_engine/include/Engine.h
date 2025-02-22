#pragma once

#include <vulkan/vulkan.h>
#include <NarcIO.h>

#include "CommandPool.h"
#include "DescriptorPool.h"
#include "window/Window.h"
#include "EngineDebugLogger.h"
#include "Pipeline.h"
#include "devices/DeviceHandler.h"
#include "SwapChain.h"
#include "buffers/UniformBuffer.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine {
    struct Vertex;
    struct Image;

    class Engine
    {
    public:
        static Engine* getInstance();

        const DeviceHandler* getDevice() const { return &m_deviceHandler; }
        const Window* getWindow() const { return &m_window; }

        void run();

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    private:
        Window m_window;
        EngineDebugLogger m_debugLogger;

        VkInstance m_vulkanInstance;
        DeviceHandler m_deviceHandler;

        SwapChain m_swapChain;

        VkDescriptorSetLayout m_descriptorSetLayout;
        DescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;

        // VkPipeline m_graphicsPipeline;
        Pipeline m_graphicsPipeline;
        CommandPool m_commandPool;
        // VkCommandPool m_commandPool;
        // std::vector<VkCommandBuffer> m_commandBuffers
        uint32_t m_currentFrame = 0;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;

        GraphicsBuffer<Vertex> m_vertexBuffer;
        GraphicsBuffer<uint16_t> m_indexBuffer;
        std::vector<UniformBuffer> m_uniformBuffers;

        VkImage m_textureImage;
        VkDeviceMemory m_textureImageMemory;
        VkImageView m_textureImageView;
        VkSampler m_textureSampler;

    private:
        void init();

        void mainLoop();

        void cleanUp();

        void createVulkanInstance();

        void createDescriptorSetLayout();

        void createGraphicsPipeline();

        void createTextureImage();

        void createTextureSampler();

        void createImageTextureView();

        void createImage(const narc_io::Image& imageData, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags
                         properties,
                         VkImage& image, VkDeviceMemory& imageMemory);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void createUniformBuffers();

        void createDescriptorPool();

        void createDescriptorSets();

        // void createCommandBuffers();
        void createSyncObjects();

        void drawFrame();

        void updateUniformBuffer(uint32_t currentImage);

        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex);
    };
}
