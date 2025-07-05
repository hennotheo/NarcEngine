#pragma once

namespace narc_engine
{
    struct QueueFamilyIndicesVulkan
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentFamily;

        bool isComplete() const
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value();
        }
    };
}
