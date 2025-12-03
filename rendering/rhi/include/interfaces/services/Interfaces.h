//
// Created by theohenno on 12/2/25.
//

#pragma once

namespace narc_engine {
    using VulkanServiceQueryError = std::string;
    
    constexpr std::unexpected<VulkanServiceQueryError> vulkanServiceUnexpected(const std::string_view& msg)
    {
        return std::unexpected(VulkanServiceQueryError{msg});
    }
    
    template<typename T>
    using VulkanServiceQuery = std::expected<T, VulkanServiceQueryError>;
}

#include "IService.h"
#include "IInstanceService.h"
#include "ISwapchainService.h"
#include "IDeviceService.h"
