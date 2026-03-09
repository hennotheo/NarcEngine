//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    struct ImageAllocationInfo;
    class IImage;
    class IDescriptorLayout;
    class IBuffer;
    class IFence;
    class ISemaphore;
    class IPipelineLayout;
    class ISwapchain;
    class IQueue;
    class IGraphicsPipeline;
    class ICommandBufferPool;
    class IDescriptorBinding;

    class IGraphicsInstance : public narc_core::IInitialisable
    {
    public:
        ~IGraphicsInstance() noexcept override;

        NARC_PURE_VIRTUAL_GETTER(const IQueue*, getGraphicsQueue);
        NARC_PURE_VIRTUAL_GETTER(const IQueue*, getPresentQueue);

        NARC_PURE_VIRTUAL_SETTER(ApplicationInfo, setApplicationInfo);
        NARC_PURE_VIRTUAL_SETTER(PhysicalDeviceCriteria, setDeviceCriteria);

        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<ISurface>, createSurface, const IWindow* window);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<ISwapchain>, createSwapChain, const ISurface* surface);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<IPipelineLayout>, createPipelineLayout, const ISwapchain* swapChain);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<IGraphicsPipeline>, createPipeline, const IPipelineLayout* surface, const ISwapchain* swapChain);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<ISemaphore>, createSemaphore);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<IFence>, createFence);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<ICommandBufferPool>, createCommandBufferPool);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<IDescriptorLayout>, createDescriptorLayout);
        NARC_PURE_VIRTUAL_QUERY(std::vector<std::unique_ptr<IDescriptorBinding>>, createDescriptorBinding, const IDescriptorLayout* layout);

        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<IBuffer>, createBuffer, const BufferAllocationInfo& allocationInfo);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<IImage>, createImage, const ImageAllocationInfo& allocationInfo);

        NARC_PURE_VIRTUAL_CMD(waitForFences, std::span<const IFence*> fences);
        NARC_PURE_VIRTUAL_CMD(resetFences, std::span<const IFence*> fences);
        NARC_PURE_VIRTUAL_CMD(waitIdle);

        virtual void attachWindow(const IWindow* window) noexcept = 0;
    };
}
