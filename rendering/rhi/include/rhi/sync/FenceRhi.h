//
// Created by theoh on 30/05/2025.
//

#pragma once

namespace narc_engine {

    NARC_DECL_RHI_PLATFORM_TYPES(Fence);

    class NARC_ENGINE_API FenceRhi : public narc_core::IInitialisable
    {

    };

    NARC_DECL_RHI_CREATION(FenceRhi);

} // narc_engine
