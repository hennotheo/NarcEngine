#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window/Window.h"
#include "QueueFamilyIndices.h"
#include "Buffer.h"
#include "EngineDebugLogger.h"
#include "SwapChain.h"
#include "Vertex.h"

#include <vector>
#include <cstdint>
#include <fstream>



namespace NarcEngine
{
    class Engine
    {
    public:
        static Engine* GetInstance();
        
        const VkDevice& GetDevice() const { return m_device; }
        const VkPhysicalDevice& GetPhysicalDevice() const { return m_physicalDevice; }
        const Window& GetWindow() const { return m_window; }

        void Run();

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

    private:
        Window m_window;
        EngineDebugLogger m_debugLogger;

        VkInstance m_instance;
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        SwapChain m_swapChain;
        
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_graphicsPipeline;
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        uint32_t m_currentFrame = 0;

        //Sync objects -> Waiting them to make operation... cf doc vulkan
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        
        Buffer<Vertex> m_vertexBuffer;
        Buffer<uint16_t> m_indexBuffer;

    private:
        void Init();
        void MainLoop();
        void CleanUp();
        
        void CreateInstance();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateGraphicsPipeline();
        void CreateCommandPool();
        void CreateCommandBuffer();
        void CreateSyncObjects();

        void DrawFrame();
        
        int RateDeviceSuitability(VkPhysicalDevice device);
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        static std::vector<char> ReadFile(const std::string& filename);
    };
}
