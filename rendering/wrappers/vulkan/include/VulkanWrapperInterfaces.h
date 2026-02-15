//
// Created by theohenno on 12/2/25.
//

#pragma once

using VulkanServiceQueryError = std::string;

constexpr std::unexpected<VulkanServiceQueryError> vulkanServiceUnexpected(const std::string_view& msg)
{
    return std::unexpected(VulkanServiceQueryError{msg});
}

template<typename T>
using VulkanServiceQuery = std::expected<T, VulkanServiceQueryError>;

#include "buffers/interfaces/IVulkanBuffer.h"

#include "services/interfaces/IInstanceService.h"
#include "services/interfaces/ISwapchainService.h"
#include "services/interfaces/IDeviceService.h"
#include "services/interfaces/IDeviceQueueService.h"
#include "services/interfaces/IVulkanMemoryAllocationService.h"
#include "services/interfaces/ICmdService.h"
