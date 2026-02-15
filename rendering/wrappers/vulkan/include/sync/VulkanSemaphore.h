//
// Created by theohenno on 12/1/25.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;
}

namespace narc_engine
{
    class VulkanSemaphore final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanSemaphore(std::weak_ptr<VulkanDevice> device);
        ~VulkanSemaphore() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkSemaphore, getHandle, m_semaphore);
        
    private:
        std::weak_ptr<VulkanDevice> m_device;
        
        VkSemaphore m_semaphore = VK_NULL_HANDLE;
    };
} // narc_engine