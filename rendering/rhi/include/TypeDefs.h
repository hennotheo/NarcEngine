//
// Created by theohenno on 11/25/25.
//

#pragma once

namespace narc_engine {
    class DeviceQueueService;
    class PhysicalDeviceService;
    class SwapChainService;
    
    using DeviceQueueServicePtr = std::shared_ptr<DeviceQueueService>;
    using PhysicalDeviceServicePtr = std::shared_ptr<PhysicalDeviceService>;
    using SwapChainServicePtr = std::shared_ptr<SwapChainService>;
}
