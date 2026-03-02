//
// Created by theo on 3/1/26.
//

#pragma once

namespace narc_engine {
    class IFence;
    class ISemaphore;
    class ICommandBuffer;

    struct QueueSubmitInfos
    {
        std::vector<ICommandBuffer*> CommandBuffers{};
        std::vector<ISemaphore*> SignalSemaphores{};
        std::vector<ISemaphore*> WaitSemaphores{};
        const IFence* Fence{};
    };
}
