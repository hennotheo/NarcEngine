//
// Created by theo on 2/15/26.
//

#include "mapping/mappingFromVk.h"

namespace narc_engine::mapping {

    narc_math::Extent mapFromVkExtend(const VkExtent2D& extent)
    {
        return {
                extent.width,
                extent.height
        };
    }
}

