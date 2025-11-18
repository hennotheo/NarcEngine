//
// Created by theohenno on 10/22/25.
//

#pragma once

namespace narc_engine {
    using ExtensionNameList = std::vector<const char*>;

    class IVulkanExtension : public narc_core::IInitialisable
    {
    public:
        ~IVulkanExtension() noexcept override = default;

        NARC_PURE_VIRTUAL_GETTER(ExtensionNameList, getExtensionNames);
        NARC_PURE_VIRTUAL_GETTER(const void*, getCreationInfos);
    };
}
