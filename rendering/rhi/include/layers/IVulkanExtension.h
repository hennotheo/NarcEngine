//
// Created by theohenno on 10/22/25.
//

#pragma once

#include "VulkanInstance.h"

namespace narc_engine {
    class IVulkanExtension : public narc_core::IInitialisable
    {
    public:
        explicit IVulkanExtension(std::weak_ptr<VulkanInstance>& instance) noexcept :
            m_instance(std::move(instance))
        {
        }

        ~IVulkanExtension() noexcept override = default;

        NARC_PURE_VIRTUAL_GETTER(constexpr const char*, getExtensionName);
        NARC_PURE_VIRTUAL_GETTER(const void*, getCreationInfos);

    protected:
        std::weak_ptr<VulkanInstance> m_instance;
    };
}
