//
// Created by theo on 2/17/26.
//

#pragma once

#include "device/VulkanDevice.h"

namespace narc_engine {
    struct ImageAllocationInfo;
    class VulkanSwapChain;
    class VulkanRenderPass;
    class VulkanMemoryAllocator;
    class VulkanDescriptorSetLayout;
    class VulkanDescriptorPool;
    class IDescriptorBinding;

    class VulkanGraphicsInstance : public IGraphicsInstance
    {
    public:
        VulkanGraphicsInstance();
        ~VulkanGraphicsInstance() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(const IQueue*, getGraphicsQueue, m_device->getGraphicsQueue())
        NARC_OVERRIDE_GETTER(const IQueue*, getPresentQueue, m_device->getPresentQueue())

        void setApplicationInfo(const ApplicationInfo& value) noexcept override;
        void setDeviceCriteria(const PhysicalDeviceCriteria& value) noexcept override;

        NARC_QUERY_OVERRIDE(std::unique_ptr<ISurface>, createSurface, const IWindow* window);
        NARC_QUERY_OVERRIDE(std::unique_ptr<ISwapchain>, createSwapChain, const ISurface* surface);
        NARC_QUERY_OVERRIDE(std::unique_ptr<IPipelineLayout>, createPipelineLayout, const ISwapchain* swapChain);
        NARC_QUERY_OVERRIDE(std::unique_ptr<IGraphicsPipeline>, createPipeline, const IPipelineLayout* layout, const ISwapchain* swapChain);
        NARC_QUERY_OVERRIDE(std::unique_ptr<ISemaphore>, createSemaphore);
        NARC_QUERY_OVERRIDE(std::unique_ptr<IFence>, createFence);
        NARC_QUERY_OVERRIDE(std::unique_ptr<ICommandBufferPool>, createCommandBufferPool);
        NARC_QUERY_OVERRIDE(std::unique_ptr<IDescriptorLayout>, createDescriptorLayout);
        NARC_QUERY_OVERRIDE(std::vector<std::unique_ptr<IDescriptorBinding>>, createDescriptorBinding, const IDescriptorLayout* layout);

        NARC_QUERY_OVERRIDE(std::unique_ptr<IBuffer>, createBuffer, const BufferAllocationInfo& allocationInfo);
        NARC_QUERY_OVERRIDE(std::unique_ptr<IImage>, createImage, const ImageAllocationInfo& allocationInfo);

        NARC_CMD_OVERRIDE(waitForFences, std::span<const IFence*> fences);
        NARC_CMD_OVERRIDE(resetFences, std::span<const IFence*> fences);
        NARC_CMD_OVERRIDE(waitIdle);

        void attachWindow(const IWindow* window) noexcept override;

    private:
        const IWindow* m_mainWindow;

        std::unique_ptr<VulkanInstance> m_instance;
        std::unique_ptr<VulkanDevice> m_device;
        std::unique_ptr<VulkanDescriptorPool> m_descriptorPool;
        mutable std::unordered_map<const VulkanSwapChain*, std::unique_ptr<VulkanRenderPass>> m_swapChainRenderPasses;
    };
}
