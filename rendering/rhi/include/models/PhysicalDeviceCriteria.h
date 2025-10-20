//
// Created by theohenno on 10/20/25.
//

#pragma once

namespace narc_engine {
    struct PhysicalDeviceCriteria
    {
        // Requirements
        bool RequireGeometryShader{true};
        bool RequireDiscreteGPU{true};

        // Preferences
        bool PreferDiscreteGPU{true};
    };
} // namespace narc_engine