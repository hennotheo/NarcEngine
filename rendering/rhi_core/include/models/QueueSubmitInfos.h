//
// Created by theo on 3/1/26.
//

#pragma once

namespace narc_engine {
    class IFence;
    class ISemaphore;
    class ICommandBuffer;

    enum class SubmitWaitStageMask : uint32_t
    {
        None = 0,
        ColorAttachmentOutput = 1 << 0,
        VertexShader = 1 << 1,
        FragmentShader = 1 << 2,
        ComputeShader = 1 << 3,
    };

    inline SubmitWaitStageMask operator|(
            SubmitWaitStageMask a,
            SubmitWaitStageMask b)
    {
        return static_cast<SubmitWaitStageMask>(
            static_cast<uint32_t>(a) |
            static_cast<uint32_t>(b));
    }

    struct QueueSubmitInfos
    {
        SubmitWaitStageMask WaitStages{};
        std::vector<ICommandBuffer*> CommandBuffers{};
        std::vector<ISemaphore*> SignalSemaphores{};
        std::vector<ISemaphore*> WaitSemaphores{};
        const IFence* Fence{};
    };
}
