//
// Created by theoh on 5/23/2025.
//

#pragma once

namespace narc_engine
{
    class DebuggerRhi : public narc_core::IInitialisable
    {
    public:
        DebuggerRhi();
        ~DebuggerRhi() override;
    };

    NARC_DECL_RHI_CREATION(DebuggerRhi);
} // namespace narc_engine