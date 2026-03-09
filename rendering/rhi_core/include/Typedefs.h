//
// Created by theo on 2/15/26.
//

#pragma once

namespace narc_engine {
    using SurfaceExtent = narc_math::Extent;
    using Memory = void;
    using MemorySize = uint64_t;
    using SurfacePtr = void*;
    using ImageIndex = uint32_t;

    using RhiQueryError = std::string;

    constexpr std::unexpected<RhiQueryError> RhiUnexpected(const std::string_view& msg)
    {
        return std::unexpected(RhiQueryError{msg});
    }

    template<typename T>
    using RhiQuery = std::expected<T, RhiQueryError>;

    struct QueuePresentResult
    {
        bool HasError{false};
        bool IsOutOfDate{false};
        bool IsSuboptimal{false};
    };

    struct SwapchainAcquireImageResult
    {
        ImageIndex ImageIndex;

        bool HasError{false};
        bool IsOutOfDate{false};
        bool IsSuboptimal{false};
    };

    enum VertexAttributeFormat
    {
        Float,
        Float2,
        Float3,
        Float4,
    };

    //TODO: MOVE LATER
    struct VertexAttribute
    {
        uint32_t Location;
        uint32_t Binding;
        VertexAttributeFormat Format;
        uint32_t Offset;
    };

    struct VertexLayout
    {
        std::vector<VertexAttribute> Attributes;
        uint32_t Stride;
    };

    enum ShaderStage
    {
        VertexStage = 0b0001,
        FragmentStage = 0b0010
    };

    enum DescriptorType
    {
        Sampler,
        UniformBuffer
    };

    struct DescriptorSetBindingInfo
    {
        uint32_t BindingIndex;
        ShaderStage Stage;
        DescriptorType Type;
    };

    enum ImageLayout
    {
        Undefined,
        TransferSource,
        TransferDestination,
        ShaderReadOnly
    };
}
