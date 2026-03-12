//
// Created by theo on 2/18/26.
//

#include "RhiFactory.h"

#include <RhiCore.h>
#include <NarcVulkanWrapper.h>
#include <NarcGlfwWrapper.h>

namespace narc_engine {
    std::unique_ptr<IGraphicsInstance> createVulkanGraphicsInstance()
    {
        return std::make_unique<VulkanGraphicsInstance>();
    }

    std::unique_ptr<IGraphicsInstance> createGraphicsInstance(const GraphicBackend backend)
    {
        switch (backend)
        {
            case Vulkan: return createVulkanGraphicsInstance();

            case OpenGL: return nullptr;
        }

        return nullptr;
    }

    std::unique_ptr<IWindow> createWindow(const WindowBackend backend)
    {
        switch (backend)
        {
            case Glfw: return std::make_unique<GlfwWindow>();
        }

        return nullptr;
    }
}
