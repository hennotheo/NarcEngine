//
// Created by theohenno on 10/20/25.
//

#pragma once

namespace narc_engine {
    class IVulkanExtension;
    
    struct PhysicalDeviceCriteria
    {
        // Requirements

        bool RequireGeometryShader{true};
        bool RequireDiscreteGPU{true};

        std::vector<std::shared_ptr<IVulkanExtension>> DeviceRequiredExtensions{};


        // Preferences

        bool PreferDiscreteGPU{true};
    };
} // namespace narc_engine
