//
// Created by theohenno on 12/2/25.
//

#pragma once

namespace narc_engine {
    struct ApplicationInfo;
    class IVulkanExtension;
    
    class IInstanceService : public IService
    {
    public:
        ~IInstanceService() override = default;

        NARC_PURE_VIRTUAL_MAP(ApplicationInfo, VkApplicationInfo);

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VkInstance>, createVkInstance,
                                const VkApplicationInfo& appInfos,
                                const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                                const std::span<const char*>& layerNames);

        virtual void destroyVkInstance(const VkInstance& instance) const noexcept =0;
    };
}
