//
// Created by theohenno on 10/21/25.
//

#pragma once

#include "models/QueueFamilyIndices.h"

namespace narc_engine {
    using QueryQueueError = std::string;

    class DeviceQueueService final
    {
    public:
        explicit DeviceQueueService();
        ~DeviceQueueService();

        QUERY(QueueFamilyIndices, QueryQueueError) queryQueueFamilyIndices(const VkPhysicalDevice& physicalDevice) const;

        NO_DISCARD std::vector<QueueFamilyIndex> getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) const;
    };
} // narc_engine
