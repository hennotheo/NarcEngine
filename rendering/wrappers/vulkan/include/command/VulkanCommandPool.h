//
// Created by theohenno on 11/30/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandBuffer;
    class VulkanDevice;
    
    class VulkanCommandPool final : public ICommandBufferPool
    {
    public:
        explicit VulkanCommandPool(const VulkanDevice* device);
        ~VulkanCommandPool() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_QUERY_OVERRIDE(RhiQuery<std::unique_ptr<ICommandBuffer>>, allocateOneTimeBuffer);
        NARC_QUERY_OVERRIDE(RhiQuery<std::unique_ptr<ICommandBuffer>>, allocateCommandBuffer);

        NARC_GETTER(VkCommandPool, getHandle, m_commandPool);

        void freeBuffer(const VulkanCommandBuffer* buffers);
        void destroyOneTimeBuffer(const ICommandBuffer* cmd) override;

    private:
        const VulkanDevice* m_device;
        
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
    };
} // narc_engine
