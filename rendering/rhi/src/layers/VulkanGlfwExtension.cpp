//
// Created by theohenno on 11/18/25.
//

#include "layers/VulkanGlfwExtension.h"

#include <GLFW/glfw3.h>

namespace narc_engine {
    VulkanGlfwExtension::VulkanGlfwExtension()
    {
        glfwInit();
    }

    VulkanGlfwExtension::~VulkanGlfwExtension()
    {
        glfwTerminate();
    }

    void VulkanGlfwExtension::init()
    {
        //Nothing to initialize.
    }

    void VulkanGlfwExtension::shutdown()
    {
        //Nothing to shutdown.
    }

    ExtensionNameList VulkanGlfwExtension::queryGlfwExtensions() const
    {
        uint32_t count = 0;
        const auto requiredExtensions = glfwGetRequiredInstanceExtensions(&count);
        std::vector<const char*> extensions{};
        extensions.reserve(count);
        
        for (int i = 0; i < count; ++i)
        {
            extensions.push_back(requiredExtensions[i]);
        }

        return extensions;
    }
} // narc_engine
