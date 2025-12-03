//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "IVulkanDeviceConfigProvider.h"

#include "models/PhysicalDeviceCriteria.h"

namespace narc_engine {
    class IVulkanExtension;

    class EngineConfigProvider final :
            public IVulkanDeviceConfigProvider
    {
    public:
        explicit EngineConfigProvider() = default;
        ~EngineConfigProvider() override = default;
        
        NARC_OVERRIDE_GETTER(const PhysicalDeviceCriteria&, getPhysicalDeviceCriteria, m_physicalDeviceCriteria)
        
        std::vector<std::unique_ptr<IVulkanExtension>> m_extensionNames;
        PhysicalDeviceCriteria m_physicalDeviceCriteria;
    };
} // narc_engine
