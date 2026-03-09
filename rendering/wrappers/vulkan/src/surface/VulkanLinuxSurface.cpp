//
// Created by theo on 2/19/26.
//

#include "surface/VulkanLinuxSurface.h"

#include "instance/VulkanInstance.h"

namespace narc_engine {

    VulkanLinuxSurface::VulkanLinuxSurface(const VulkanInstance* instance, const IWindow* window) :
        m_window(window),
        m_instance(instance)
    {
    }

    VulkanLinuxSurface::~VulkanLinuxSurface() = default;

    void VulkanLinuxSurface::init()
    {
        const auto nativeHandle = m_window->getNativeHandle();

        VkXcbSurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.window = nativeHandle.Window;
        createInfo.connection = nativeHandle.Connection;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;

        if (vkCreateXcbSurfaceKHR(m_instance->getHandled(), &createInfo, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Could not create Vulkan XCB surface!");
        }

        NARC_LOG_DEBUG("Created Vulkan XCB surface!");
    }

    void VulkanLinuxSurface::shutdown()
    {
        vkDestroySurfaceKHR(m_instance->getHandled(), m_surface, nullptr);
    }
}

