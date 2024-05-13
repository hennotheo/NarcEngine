#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <vector>
#include <map>
#include <optional>
#include <set>

namespace NarcEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		bool IsComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
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

		int RateDeviceSuitability(VkPhysicalDevice device);
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	};
}