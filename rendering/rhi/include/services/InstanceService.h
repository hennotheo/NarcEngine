//
// Created by theohenno on 12/3/25.
//

#pragma once

#include "interfaces/services/IInstanceService.h"

namespace narc_engine {
    struct ApplicationInfo;

    class InstanceService final : public IInstanceService
    {
    public:
        InstanceService() = default;
        ~InstanceService() override = default;

        NARC_MAP_OVERRIDE(ApplicationInfo, VkApplicationInfo);

        NARC_QUERY_OVERRIDE(VulkanCreationQuery<VkInstance>, createVkInstance,
                            const VkApplicationInfo& appInfos,
                            const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                            const std::span<const char*>& layerNames);

        void destroyVkInstance(const VkInstance& instance) const noexcept override;
    };
} // narc_engine
