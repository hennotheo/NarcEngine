//
// Created by theo on 2/15/26.
//

#pragma once

namespace narc_engine {
    using SurfaceExtent = narc_math::Extent;
    using Memory = void;
    using MemorySize = uint64_t;
    using SurfacePtr = void*;

    using RhiQueryError = std::string;

    constexpr std::unexpected<RhiQueryError> RhiUnexpected(const std::string_view& msg)
    {
        return std::unexpected(RhiQueryError{msg});
    }

    template<typename T>
    using RhiQuery = std::expected<T, RhiQueryError>;
}
