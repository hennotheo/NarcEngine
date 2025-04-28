//
// Created by theoh on 3/2/2025.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/EngineBuilder.h"

namespace narc_engine {
    class EngineInstance final
    {
    public:
        explicit EngineInstance(const EngineBuilder* builder);
        ~EngineInstance();

        GETTER VkInstance getvkInstance() const { return m_instance; }

    private:
        VkInstance m_instance;
        VkApplicationInfo m_appInfo{};

    private:
        VkApplicationInfo createAppInfo();
        uint32_t getInstanceVersion();
        void checkValidationLayerSupport(LayersPtr validationLayers);
        VkInstanceCreateInfo getInstanceCreateInfo(VkApplicationInfo* appinfos);
        std::vector<const char*> getRequiredExtensions();
    };
} // narc_engine