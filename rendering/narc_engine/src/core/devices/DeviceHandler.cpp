#include "core/devices/DeviceHandler.h"

#include <NarcLog.h>

#include "core/EngineBuilder.h"
#include "core/EngineInstance.h"
#include "core/interfaces/ISurfaceProvider.h"
#include "core/queues/QueueFamilyIndices.h"
#include "core/EngineDebugLogger.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace narc_engine
{
    DeviceHandler::DeviceHandler(const EngineBuilder* builder)
    {
        m_instance = builder->getInstance();
        m_surface = builder->getSurfaceProvider();
        m_deviceExtensions = builder->getDeviceExtensions();

        m_physicalDevice = std::make_unique<PhysicalDevice>(builder);
        m_logicalDevice = std::make_unique<LogicalDevice>(builder, m_physicalDevice.get());

        // QueueFamilyIndices indices = m_physicalDevice->getQueueFamilyIndices();
        // vkGetDeviceQueue(m_logicalDevice->getVkDevice(), indices.GraphicsFamily.value(), 0, &m_graphicsQueue);
        // vkGetDeviceQueue(m_logicalDevice->getVkDevice(), indices.PresentFamily.value(), 0, &m_presentQueue);
    }

    DeviceHandler::~DeviceHandler()
    {

    }
}