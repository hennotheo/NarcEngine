//
// Created by theohenno on 11/30/25.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;
    
    class VulkanCommandPool final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanCommandPool(std::weak_ptr<VulkanDevice> device);
        ~VulkanCommandPool() noexcept override;

        NARC_IMPL_INITIALISABLE();

    private:
        std::weak_ptr<VulkanDevice> m_device;
        
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
    };
} // narc_engine
