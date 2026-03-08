//
// Created by theo on 10/30/25.
//

#pragma once

namespace narc_engine {
    class VulkanFence;
    class VulkanDevice;

    class VulkanQueue final : public narc_core::IInitialisable, public IQueue
    {
    public:
        explicit VulkanQueue();
        ~VulkanQueue() override;

        NARC_IMPL_INITIALISABLE();

        void setQueueFamilyIndex(const QueueFamilyIndex index) { m_queueFamilyIndex = index; }
        void setQueueIndex(const QueueIndex index) { m_queueIndex = index; }
        void setDevice(const VulkanDevice* device) { m_device = device; }
        
        VkResult submit(uint32_t submitCount, const VkSubmitInfo& infos, const VulkanFence* fence) const;
        void waitIdle() const;

        NARC_GETTER(VkQueue, getHandle, m_queue);

        NARC_CMD_OVERRIDE(submit, QueueSubmitInfos infos);
        NARC_CMD_OVERRIDE(present, QueuePresentInfos infos);
        NARC_CMD_OVERRIDE(waitQueueIdle);

    private:
        const VulkanDevice* m_device = nullptr;

        QueueFamilyIndex m_queueFamilyIndex;
        QueueIndex m_queueIndex;

        VkQueue m_queue = VK_NULL_HANDLE;

        bool isIndexDefined(const uint32_t& index);
        bool checkIndicesDefined();
    };
} // namespace narc_engine
