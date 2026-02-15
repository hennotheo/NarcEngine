//
// Created by theohenno on 11/25/25.
//

#pragma once

namespace narc_engine {
    struct SwapChainSupportInfoVulkan
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };
}
