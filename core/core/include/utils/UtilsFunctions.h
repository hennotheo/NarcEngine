//
// Created by theo on 2/15/26.
//

#pragma once

namespace narc_core {
    template<typename T>
    static T getAndCastHandle(const IHandler* handler)
    {
        return static_cast<T>(handler->getHandle());
    }
}
