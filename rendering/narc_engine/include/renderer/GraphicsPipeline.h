//
// Created by theoh on 3/16/2025.
//

#pragma once
#include <vulkan/vulkan_core.h>

#include "SwapChain.h"
#include "core/DeviceComponent.h"

namespace narc_engine {
    class CommandBuffer;

    class GraphicsPipeline : public DeviceComponent
    {
    public:
        explicit GraphicsPipeline(const SwapChain* swapChain, const VkDescriptorSetLayout* descriptorSetLayout);
        ~GraphicsPipeline();

        GETTER VkPipelineLayout getLayout() const { return m_pipelineLayout; }

        void bindPipeline(const CommandBuffer* commandBuffer) const;

    private:

        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        void createPipelineLayout(const VkDescriptorSetLayout* descriptorSetLayout);
        void createPipeline(const SwapChain* swapChain);
    };
}
