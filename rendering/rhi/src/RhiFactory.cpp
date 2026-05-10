//
// Created by theo on 2/18/26.
//

#include "factory/RhiFactory.h"

#include <NarcVulkanWrapper.h>
#include <NarcGlfwWrapper.h>

#include "ImguiVulkanWrapper.h"
#include "NarcImguiWrapper.h"
#include "models/GuiInitContext.h"

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

    std::unique_ptr<ImGuiBackend> createGuiBackend(const GraphicBackend backend, const GuiInitContext& context)
    {
        switch (backend)
        {
            case Vulkan: return std::make_unique<ImguiVulkanWrapper>(context);
        }

        NARC_ERROR_NOT_IMPLEMENTED("Backend not implemented.");
    }

    std::unique_ptr<IGui> createGui(const GraphicBackend& backend, const GuiInitContext& context)
    {
        auto backendInstance = createGuiBackend(backend, context);

        return std::make_unique<ImguiWrapper>(
            std::move(backendInstance)
        );
    }
}
