//
// Created by theo on 2/17/26.
//

#include "instance/VulkanGraphicsInstance.h"

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
        m_device->init();
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
}
