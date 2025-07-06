//
// Created by theoh on 7/6/2025.
//

#pragma once

#include "queue/PresentQueueRhi.h"

#include "DeviceRhi.h"
#include "backend_vulkan/DeviceVulkan.h"

namespace narc_engine
{
    class PresentQueueVulkan final : public PresentQueueRhi
    {
    public:
        explicit PresentQueueVulkan(const DeviceRhiPtr& device);
        ~PresentQueueVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_IMPL_RHI_PLATFORM_GETTER(PresentQueue, Vulkan);

        void present() const override;

    protected:
        NARC_OVERRIDE_GETTER(DeviceVulkan*, getDevice, super::getDevice ()->getDeviceVulkan())

    private:
        VkQueue m_queue = VK_NULL_HANDLE;

        typedef PresentQueueRhi super;
    };
} // narc_engine
