//
// Created by theo on 10/30/25.
//

#pragma once

namespace narc_engine {
    class VulkanFence;
    class VulkanDevice;

    class VulkanQueue final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanQueue();
        ~VulkanQueue() override;

        NARC_IMPL_INITIALISABLE();

        void setQueueFamilyIndex(const QueueFamilyIndex index) { m_queueFamilyIndex = index; }
        void setQueueIndex(const QueueIndex index) { m_queueIndex = index; }
        void setDevice(std::weak_ptr<VulkanDevice> device) { m_device = std::move(device); }
        
        VkResult submit(uint32_t submitCount, const VkSubmitInfo& infos, const VulkanFence* fence) const;
        void waitIdle() const;

        NARC_GETTER(VkQueue, getHandle, m_queue);

    private:
        std::weak_ptr<VulkanDevice> m_device;

        QueueFamilyIndex m_queueFamilyIndex = QUEUE_INDEX_NONE;
        QueueIndex m_queueIndex = QUEUE_INDEX_NONE;

        VkQueue m_queue = VK_NULL_HANDLE;

        static void assertIndexDefined(const uint32_t& index);
    };
} // namespace narc_engine
