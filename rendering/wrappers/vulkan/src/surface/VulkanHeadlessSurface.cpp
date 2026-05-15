//
// Created by theo on 5/15/26.
//

#include "surface/VulkanHeadlessSurface.h"

#include "instance/VulkanInstance.h"

namespace narc_engine {

    VulkanHeadlessSurface::VulkanHeadlessSurface(const VulkanInstance* instance) :
        m_instance(instance)
    {
    }

    VulkanHeadlessSurface::~VulkanHeadlessSurface()
    {
    }

    void VulkanHeadlessSurface::init()
    {
        constexpr VkHeadlessSurfaceCreateInfoEXT surfaceInfo = {
                .sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT,
                .pNext = nullptr,
                .flags = 0,
        };

        if (vkCreateHeadlessSurfaceEXT(m_instance->getHandled(), &surfaceInfo, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Could not create Vulkan XCB surface!");
        }

        NARC_LOG_DEBUG("Created Vulkan XCB surface!");
        initialized = true;
    }

    void VulkanHeadlessSurface::shutdown()
    {
        vkDestroySurfaceKHR(m_instance->getHandled(), m_surface, nullptr);
        initialized = false;
    }
}
