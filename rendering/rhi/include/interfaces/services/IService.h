//
// Created by theohenno on 12/3/25.
//

#pragma once

namespace narc_engine {
    class IService
    {
    public:
        virtual ~IService() = default;
    };

    using VulkanServiceQueryError = std::string;

    constexpr std::unexpected<VulkanServiceQueryError> vulkanServiceUnexpected(const std::string_view& msg)
    {
        return std::unexpected(VulkanServiceQueryError{msg});
    }

    template<typename T>
    using VulkanServiceQuery = std::expected<T, VulkanServiceQueryError>;
}