//
// Created by theohenno on 11/30/25.
//

#pragma once

#include "VulkanCommandBuffer.h"

namespace narc_engine {
    class VulkanDevice;
    
    class VulkanCommandPool final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanCommandPool(std::weak_ptr<VulkanDevice> device);
        ~VulkanCommandPool() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkCommandPool, getHandle, m_commandPool);
        
        std::unique_ptr<VulkanCommandBuffer> allocateCopyBuffer() const;
        std::unique_ptr<VulkanCommandBuffer> allocateCommandBuffer() const;
        
        // void freeBuffers(std::vector<VulkanCommandBuffer> buffers);
        void freeBuffer(const VulkanCommandBuffer& buffers);

    private:
        std::weak_ptr<VulkanDevice> m_device;
        
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
    };
} // narc_engine
