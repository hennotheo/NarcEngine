//
// Created by theo on 2/17/26.
//

#include "instance/VulkanGraphicsInstance.h"

#include "instance/VulkanInstance.h"
#include "surface/VulkanLinuxSurface.h"
#include "swapchain/VulkanSwapChain.h"

namespace narc_engine {
    VulkanGraphicsInstance::VulkanGraphicsInstance() :
        m_mainWindow(nullptr)
    {
        m_instance = std::make_unique<VulkanInstance>();
        m_device = std::make_unique<VulkanDevice>(m_instance.get());
    }

    VulkanGraphicsInstance::~VulkanGraphicsInstance() noexcept = default;

    void VulkanGraphicsInstance::init()
    {
        m_instance->init();

        const auto surface = createSurface(m_mainWindow);
        surface->init();

        m_device->setMainWindowSurface(static_cast<IVulkanSurface*>(surface.get()));
        m_device->init();

        surface->shutdown();
    }

    void VulkanGraphicsInstance::shutdown()
    {
        m_device->shutdown();
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

    std::unique_ptr<ISwapchain> VulkanGraphicsInstance::createSwapChain(const ISurface* surface) const noexcept
    {
        return std::make_unique<VulkanSwapChain>(m_device.get(), static_cast<const IVulkanSurface*>(surface));
    }

    std::unique_ptr<ISurface> VulkanGraphicsInstance::createSurface(const IWindow* window) const noexcept
    {
        return std::make_unique<VulkanLinuxSurface>(m_instance.get(), window);
    }

    void VulkanGraphicsInstance::attachWindow(const IWindow* window) noexcept
    {
        m_mainWindow = window;
    }
}
