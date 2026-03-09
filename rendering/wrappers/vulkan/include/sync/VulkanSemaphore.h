//
// Created by theohenno on 12/1/25.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;
}

namespace narc_engine
{
    class VulkanSemaphore final : public ISemaphore
    {
    public:
        explicit VulkanSemaphore(const VulkanDevice* device);
        ~VulkanSemaphore() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkSemaphore, getHandle, m_semaphore);
        
    private:
        const VulkanDevice* m_device;
        
        VkSemaphore m_semaphore = VK_NULL_HANDLE;
    };
} // narc_engine