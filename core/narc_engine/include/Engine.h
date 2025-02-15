#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "window/Window.h"

#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <vector>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <array>
#include <fstream>

#include "Buffer.h"
#include "EngineDebugLogger.h"


namespace NarcEngine
{
    class Engine
    {
    public:
        static Engine* GetInstance();
        
        const VkDevice& GetDevice() const { return m_device; }

        void Run();

        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    private:
        Window m_window;
        EngineDebugLogger m_debugLogger;

        VkInstance m_instance;
        // VkDebugUtilsMessengerEXT m_debugMessenger;
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        VkRenderPass m_renderPass;
        VkPipelineLayout m_pipelineLayout;
        VkPipeline m_graphicsPipeline;
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        uint32_t m_currentFrame = 0;

        //Sync objects -> Waiting them to make operation... cf doc vulkan
        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;

        // VkBuffer m_vertexBuffer;
        // VkDeviceMemory m_vertexBufferMemory;
        Buffer<Vertex> m_vertexBuffer;
        Buffer<uint16_t> m_indexBuffer;
        // VkBuffer m_indexBuffer;
        // VkDeviceMemory m_indexBufferMemory;

    private:
        // Engine() = default;
        // ~Engine() = default;

        // Engine(const Engine&) = delete;
        // Engine& operator=(const Engine&) = delete;

        void InitVulkan();
        void MainLoop();
        void CleanupSwapChain();
        void CleanUp();

        // void InitWindow();
        void CreateInstance();
        //void SetupDebugMessenger();
        // void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void RecreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateGraphicsPipeline();
        void CreateCommandPool();
        // void CreateVertexBuffer();
        // void CreateIndexBuffer();
        void CreateCommandBuffer();
        void CreateSyncObjects();

        void DrawFrame();

        // void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        // bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        int RateDeviceSuitability(VkPhysicalDevice device);
        // bool CheckValidationLayerSupport();
        // std::vector<const char*> GetRequiredExtensions();
        // void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        // SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        // static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        //     VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        //     VkDebugUtilsMessageTypeFlagsEXT messageType,
        //     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        //     void* pUserData);
        static std::vector<char> ReadFile(const std::string& filename);
    };
}
