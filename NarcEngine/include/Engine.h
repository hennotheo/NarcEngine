#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <stdlib.h>

class Engine
{
public:
	void Run();

private:
	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void CleanUp();
	void CreateInstance();

private:
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;

	GLFWwindow* m_window;
	VkInstance m_instance;
};