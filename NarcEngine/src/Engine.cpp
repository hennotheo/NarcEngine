#include "Engine.h"

#include <vector>

void Engine::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	CleanUp();
}

void Engine::InitWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Narcoleptic Engine", nullptr, nullptr);
}

void Engine::InitVulkan()
{
	CreateInstance();
}

void Engine::MainLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
	}
}

void Engine::CleanUp()
{
	vkDestroyInstance(m_instance, nullptr);
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Engine::CreateInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Narcoleptic Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;
	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:\n";
	for (const auto& extension : extensions)
	{
		std::cout << '\t' << extension.extensionName << '\n';
	}
}