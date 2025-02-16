#pragma once

#include <vulkan/vulkan.h>

#include "Vertex.h"
#include "QueueFamilyIndices.h"
#include "buffers/UniformBuffer.h"

#include "window/Window.h"
#include "EngineDebugLogger.h"
#include "SwapChain.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine
{
    class Engine
    {
    public:
        static Engine* getInstance();

        const VkDevice& getDevice() const { return m_device; }
        const VkPhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
        const Window* getWindow() const { return &m_window; }

        void run();

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    private:
        Window m_window;
        EngineDebugLogger m_debugLogger;

        VkInstance m_instance;
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        SwapChain m_swapChain;

        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_graphicsPipeline;
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
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

        void createInstance();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createCommandPool();
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void createTextureImage();
        void createTextureSampler();
        void createImageTextureView();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                         VkDeviceMemory& imageMemory);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createCommandBuffers();
        void createSyncObjects();

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);

        int rateDeviceSuitability(VkPhysicalDevice device);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        static std::vector<char> readFile(const std::string& filename);
    };
}
