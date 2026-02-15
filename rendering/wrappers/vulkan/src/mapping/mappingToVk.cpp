//
// Created by theo on 2/15/26.
//

#include "mapping/mappingToVk.h"

namespace narc_engine::mapping {

    VkExtent2D mapFromExtend(const narc_math::Extent& extent)
    {
        return {
                extent.Width,
                extent.Height
        };
    }
}

