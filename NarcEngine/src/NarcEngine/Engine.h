#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
#include <fstream>

namespace NarcEngine
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentFamily;

        bool IsComplete()
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    class Engine
    {
    public:
        void Run();

    private:
        GLFWwindow* m_window;
        VkInstance m_instance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface;

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

    private:
        void InitWindow();
        void InitVulkan();
        void MainLoop();
        void CleanUp();

        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateGraphicsPipeline();
        void CreateFramebuffers();
        void CreateCommandPool();
        void CreateCommandBuffer();
        void CreateSyncObjects();

        void DrawFrame();

        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        int RateDeviceSuitability(VkPhysicalDevice device);
        bool CheckValidationLayerSupport();
        std::vector<const char*> GetRequiredExtensions();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
        static std::vector<char> ReadFile(const std::string& filename);
    };
}
