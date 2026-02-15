//
// Created by theohenno on 11/18/25.
//

#pragma once

#include <NarcRenderingCore.h>
#include <NarcVulkanWrapper.h>

class GLFWwindow;

namespace narc_engine {
    class GlfwVulkanSurface final : public ISurface
    {
    public:
        explicit GlfwVulkanSurface(std::weak_ptr<VulkanInstance> instance);
        ~GlfwVulkanSurface() override = default;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(SurfacePtr, getHandle, static_cast<SurfacePtr>(m_surface)); //TODO: Create IWindow later
        NO_DISCARD bool shouldClose() const noexcept override;
        NO_DISCARD SurfaceExtend getSurfaceExtent() const noexcept override;

    private:
        std::weak_ptr<VulkanInstance> m_instance;

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        GLFWwindow* m_window = nullptr;
    };
} // narc_engine
