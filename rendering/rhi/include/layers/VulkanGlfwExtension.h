//
// Created by theohenno on 11/18/25.
//

#pragma once

#include "device/IVulkanExtension.h"

namespace narc_engine
{
    class VulkanGlfwExtension final : public IVulkanExtension
    {
    public:
        VulkanGlfwExtension();
        ~VulkanGlfwExtension() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(ExtensionNameList, getExtensionNames, queryGlfwExtensions())
        NARC_OVERRIDE_GETTER(const void*, getCreationInfos, nullptr)

    private:
        NO_DISCARD ExtensionNameList queryGlfwExtensions() const;
    };
} // narc_engine