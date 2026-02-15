//
// Created by theohenno on 12/1/25.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;

    class VulkanFence final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanFence(std::weak_ptr<VulkanDevice> device);
        ~VulkanFence() override;

        NARC_IMPL_INITIALISABLE();

        void wait(uint64_t timeout = UINT64_MAX) const;
        void reset() const;

        NARC_GETTER(VkFence, getHandle, m_fence);
        
    private:
        std::weak_ptr<VulkanDevice> m_device;
        
        VkFence m_fence = VK_NULL_HANDLE;
    };
} // narc_engine
