//
// Created by theohenno on 10/15/25.
//

#pragma once

namespace narc_engine {
    class IVulkanInstanceConfigProvider
    {
    public:
        virtual ~IVulkanInstanceConfigProvider() = default;

        NARC_PURE_VIRTUAL_GETTER(const std::string&, getApplicationName)
        NARC_PURE_VIRTUAL_GETTER(const std::string&, getEngineName)
    };
}