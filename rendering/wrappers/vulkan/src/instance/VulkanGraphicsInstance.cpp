//
// Created by theo on 2/17/26.
//

#include "instance/VulkanGraphicsInstance.h"

#include "surface/ISurface.h"
#include "surface/VulkanLinuxSurface.h"

namespace narc_engine {
    VulkanGraphicsInstance::VulkanGraphicsInstance()
    {
        m_instance = std::make_unique<VulkanInstance>();
        m_device = std::make_unique<VulkanDevice>(m_instance.get());
    }

    VulkanGraphicsInstance::~VulkanGraphicsInstance() = default;

    void VulkanGraphicsInstance::init()
    {
        m_instance->init();

        if (m_surface != nullptr)
        {
            m_surface->init();
        }

        m_device->init();
    }

    void VulkanGraphicsInstance::shutdown()
    {
        m_device->shutdown();

        if (m_surface != nullptr)
        {
            m_surface->shutdown();
        }

        m_instance->shutdown();
    }

    void VulkanGraphicsInstance::setApplicationInfo(const ApplicationInfo& value) noexcept
    {
        m_instance->setApplicationInfo(value);
    }

    void VulkanGraphicsInstance::setDeviceCriteria(const PhysicalDeviceCriteria& value) noexcept
    {
        m_device->setPhysicalDeviceCriteria(value);
    }

    void VulkanGraphicsInstance::attachWindow(const IWindow* window) noexcept
    {
        if (m_surface != nullptr) //TODO: Implement multi surfacer
        {
            NARC_LOG_WARNING("VulkanGraphicInstance multiple surfaces not yet implemented.");
            return;
        }

#ifdef NARC_ENGINE_PLATFORM_LINUX
        m_surface = std::make_unique<VulkanLinuxSurface>(m_instance.get(), window);
#else
#error Platform surface not implemented.
#endif

        m_device->setMainWindowSurface(m_surface.get());
    }
}
