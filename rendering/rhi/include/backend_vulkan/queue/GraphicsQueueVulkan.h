//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "queue/GraphicsQueueRhi.h"

#include "DeviceRhi.h"
#include "backend_vulkan/DeviceVulkan.h"

namespace narc_engine
{
    class GraphicsQueueVulkan final : public GraphicsQueueRhi
    {
    public:
        explicit GraphicsQueueVulkan(const DeviceRhiPtr& device);
        ~GraphicsQueueVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_IMPL_RHI_PLATFORM_GETTER(GraphicsQueue, Vulkan);



        void submit() override;
        void waitIdle() override;

    protected:
        NARC_OVERRIDE_GETTER(DeviceVulkan*, getDevice, static_cast<DeviceVulkan*>(super::getDevice()))

    private:
        VkQueue m_queue = VK_NULL_HANDLE;

        typedef GraphicsQueueRhi super;
    };
} // namespace narc_engine
