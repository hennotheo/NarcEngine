//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class IBuffer;

    struct RenderPassInfos
    {
        const IGraphicsPipeline* TEMPPipeline = nullptr;
        const uint32_t TEMPImageIndex = 0;
    };

    struct ViewPortInfos
    {
        narc_math::Vec2 Position{};
        SurfaceExtent Dimensions{};
    };

    struct ScissorsInfos
    {
        narc_math::Vec2Int Offset{};
        SurfaceExtent Extent{};
    };

    class ICommandBuffer
    {
    public:
        virtual ~ICommandBuffer() = default;

        NARC_PURE_VIRTUAL_CMD(begin);
        NARC_PURE_VIRTUAL_CMD(end);
        NARC_PURE_VIRTUAL_CMD(reset);
        NARC_PURE_VIRTUAL_CMD(copyBuffer, const IBuffer* source, const IBuffer* destination, MemorySize size);
        NARC_PURE_VIRTUAL_CMD(beginRenderPass, const ISwapchain* swapChain, const RenderPassInfos& infos);
        NARC_PURE_VIRTUAL_CMD(endRenderPass);
        NARC_PURE_VIRTUAL_CMD(bindVertexBuffers, const IBuffer* buffer);
        NARC_PURE_VIRTUAL_CMD(bindIndexBuffer, const IBuffer* buffer);
        NARC_PURE_VIRTUAL_CMD(draw);
        NARC_PURE_VIRTUAL_CMD(drawIndexed, uint32_t indexCount);

        NARC_PURE_VIRTUAL_CMD(bindPipeline, const IGraphicsPipeline* pipeline);
        NARC_PURE_VIRTUAL_CMD(bindViewPort, const ViewPortInfos& viewport);
        NARC_PURE_VIRTUAL_CMD(bindScissors, const ScissorsInfos& scissors);
        NARC_PURE_VIRTUAL_CMD(bindDescriptorSets, const IPipelineLayout* layout, const IDescriptorBinding* binding);
    };
}
