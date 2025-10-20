//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "IVulkanDeviceConfigProvider.h"
#include "IVulkanInstanceConfigProvider.h"

#include "models/PhysicalDeviceCriteria.h"

namespace narc_engine {
    class EngineConfigProvider final :
            public IVulkanInstanceConfigProvider,
            public IVulkanDeviceConfigProvider
    {
    public:
        explicit EngineConfigProvider() = default;
        ~EngineConfigProvider() override = default;

        NARC_OVERRIDE_GETTER(const std::string&, getApplicationName, m_applicationName)
        NARC_OVERRIDE_GETTER(const std::string&, getEngineName, m_engineName)
        NARC_OVERRIDE_GETTER(const PhysicalDeviceCriteria&, getPhysicalDeviceCriteria, m_physicalDeviceCriteria)

        std::string m_applicationName;
        std::string m_engineName;
        PhysicalDeviceCriteria m_physicalDeviceCriteria{};
    };
} // narc_engine
