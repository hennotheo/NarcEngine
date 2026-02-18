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
        const auto injector = di::make_injector(
                di::bind<IGraphicsInstance>().to<VulkanGraphicsInstance>()
                );

        return injector.create<std::unique_ptr<IGraphicsInstance>>();
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
