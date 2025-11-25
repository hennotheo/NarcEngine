//
// Created by theohenno on 11/18/25.
//

#pragma once

#include "IVulkanSurface.h"

#include "VulkanInstance.h"

class GLFWwindow;

namespace narc_engine {
    class GlfwVulkanSurface final : public IVulkanSurface
    {
    public:
        explicit GlfwVulkanSurface(std::weak_ptr<VulkanInstance> instance);
        ~GlfwVulkanSurface() override = default;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(VkSurfaceKHR, getHandled, m_surface);
        NO_DISCARD bool shouldClose() const noexcept override;
        NO_DISCARD VkExtent2D getSurfaceExtent() const noexcept override;

    private:
        std::weak_ptr<VulkanInstance> m_instance;

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        GLFWwindow* m_window = nullptr;
    };
} // narc_engine
