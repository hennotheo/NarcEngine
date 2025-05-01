#pragma once

#include <vulkan/vulkan.h>

#include "RenderPass.h"
#include "renderer/DepthResources.h"

namespace narc_engine {
    class ISurfaceProvider;
    class DeviceHandler;
    class Window;

    class SwapChain : public DeviceComponent
    {
    public:
        GETTER const VkExtent2D& getSwapChainExtent() const { return m_swapChainExtent; }
        GETTER const RenderPass* getRenderPass() const { return m_renderPass.get(); }
        GETTER const VkSwapchainKHR& getSwapChain() const { return m_swapChain; }

        void create(ISurfaceProvider* surface);
        void createFramebuffers();

        VkRenderPassBeginInfo getRenderPassBeginInfos(uint32_t imageIndex) const;
        VkResult acquireNextImage(const VkSemaphore& semaphore, uint32_t* imageIndex);
        void reCreate();

        void cleanSwapChain();
        void cleanRenderPass();

    private:
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<ImageView> m_swapChainImageViews;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        std::unique_ptr<DepthResources> m_depthResources;
        std::unique_ptr<RenderPass> m_renderPass;

        ISurfaceProvider* m_surface;

        void createSwapChain();
        void createImageViews();
    };
}