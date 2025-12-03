//
// Created by theohenno on 12/2/25.
//

#pragma once

#include "interfaces/services/Interfaces.h"

namespace narc_engine {
    struct ApplicationInfo;
    class IVulkanExtension;
    
    class IInstanceService : public IService
    {
    public:
        virtual ~IInstanceService() override = default;

        NARC_PURE_VIRTUAL_MAP(ApplicationInfo, VkApplicationInfo);

        NARC_PURE_VIRTUAL_QUERY(VulkanCreationQuery<VkInstance>, createVkInstance,
                                const VkApplicationInfo& appInfos,
                                const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                                const std::span<const char*>& layerNames);

        virtual void destroyVkInstance(const VkInstance& instance) const noexcept =0;
    };
}
