//
// Created by theo on 2/15/26.
//

#include "mapping/mappingToVk.h"

namespace narc_engine::mapping {

    VkExtent2D mapFromExtend(const narc_math::Extent& extent) noexcept
    {
        return {
                extent.Width,
                extent.Height
        };
    }

    VkApplicationInfo mapFromApplicationInfo(const ApplicationInfo& value) noexcept
    {
        return VkApplicationInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = value.ApplicationName.c_str(),
                .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
                .pEngineName = value.EngineName.c_str(),
                .engineVersion = VK_MAKE_VERSION(0, 1, 0),
                .apiVersion = VK_API_VERSION_1_3
        };
    }
}

