//
// Created by theoh on 3/2/2025.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/EngineBuilder.h"

namespace narc_engine {
    class EngineInstance final : public narc_core::IGetter<VkInstance>
    {
    public:
        explicit EngineInstance(const EngineBuilder* builder);
        ~EngineInstance();

        NARC_IMPL_IGETTER(VkInstance, m_instance)

    private:
        VkInstance m_instance;
        VkApplicationInfo m_appInfo{};

    private:
        VkApplicationInfo createAppInfo();
        uint32_t getInstanceVersion();
        void checkValidationLayerSupport(const KeywordList* validationLayers);
        VkInstanceCreateInfo getInstanceCreateInfo(VkApplicationInfo* appinfos);
        std::vector<const char*> getRequiredExtensions();
    };
} // narc_engine