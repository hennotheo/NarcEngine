//
// Created by theoh on 27/05/2025.
//

#pragma once

#include "WindowRhi.h"

namespace narc_engine
{
    class ContextVulkan;

    class WindowVulkan final : public WindowRhi
    {
    public:
        explicit WindowVulkan(const ContextVulkan* ctx);
        ~WindowVulkan() override;

        NARC_IMPL_INITIALISABLE();

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    };
}
