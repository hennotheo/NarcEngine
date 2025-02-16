#pragma once

#include <vulkan/vulkan.h>

#include "Vertex.h"
#include "QueueFamilyIndices.h"
#include "buffers/StaggingBuffer.h"
#include "buffers/UniformBuffer.h"

#include "window/Window.h"
#include "EngineDebugLogger.h"
#include "SwapChain.h"

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

        StaggingBuffer<Vertex> m_vertexBuffer;
        StaggingBuffer<uint16_t> m_indexBuffer;

        std::vector<UniformBuffer> m_uniformBuffers;
        
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
        void createUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createCommandBuffer();
        void createSyncObjects();

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);

        int rateDeviceSuitability(VkPhysicalDevice device);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        static std::vector<char> readFile(const std::string& filename);
    };
}
