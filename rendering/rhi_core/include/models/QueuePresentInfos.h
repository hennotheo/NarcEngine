//
// Created by theo on 3/2/26.
//

#pragma once

#include "interfaces/ISwapchain.h"

namespace narc_engine {
    struct QueuePresentInfos
    {
        std::vector<ImageIndex> ImageIndices;
        std::vector<ISwapchain*> SwapChains{};
        std::vector<ISemaphore*> WaitSemaphores{};
    };
}
