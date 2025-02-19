#pragma once
#include <vulkan/vulkan_core.h>

#include "SwapChain.h"
#include "devices/DeviceHandler.h"

namespace narc_engine {
    class Pipeline
    {
        friend class CommandBuffer;

    public:
        void create(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);

        void release();

    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        const DeviceHandler* m_deviceHandler;
    };
} // narc_engine
