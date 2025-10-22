//
// Created by theohenno on 10/21/25.
//

#pragma once

namespace narc_engine {
    using QueueFamilyIndex = uint32_t;

    struct QueueFamilyIndices
    {
        std::optional<QueueFamilyIndex> GraphicsFamily;
        std::optional<QueueFamilyIndex> PresentationFamily;

        NARC_GETTER(bool, isComplete, GraphicsFamily.has_value () && PresentationFamily.has_value ());
    };
}
