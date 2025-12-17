//
// Created by theo on 12/16/25.
//

#pragma once

namespace narc_engine {
    class IVulkanBuffer
    {
    public:
        virtual ~IVulkanBuffer() = default;

        NARC_PURE_VIRTUAL_GETTER(const VkBuffer&, getHandle);
    };
}
