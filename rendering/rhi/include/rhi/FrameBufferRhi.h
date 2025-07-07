//
// Created by theoh on 7/7/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES (FrameBuffer);

    class NARC_ENGINE_API FrameBufferRhi : public narc_core::IInitialisable
    {
    public:
        FrameBufferRhi();
        ~FrameBufferRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS (FrameBuffer);
    };

    using FrameBufferRhiPtr = std::unique_ptr<FrameBufferRhi>;
}
