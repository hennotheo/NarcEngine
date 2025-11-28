//
// Created by theohenno on 11/27/25.
//

#pragma once
#include <memory>

#include "VulkanSwapChain.h"

namespace narc_engine {
    class VulkanDevice;

    class VulkanGraphicsPipeline final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanGraphicsPipeline(const std::weak_ptr<VulkanDevice>& device, const std::weak_ptr<VulkanSwapChain>& swapchain);
        ~VulkanGraphicsPipeline() override;

        NARC_IMPL_INITIALISABLE();

    private:
        std::weak_ptr<VulkanDevice> m_device;
        std::weak_ptr<VulkanSwapChain> m_swapChain;

        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    };
} // narc_engine
