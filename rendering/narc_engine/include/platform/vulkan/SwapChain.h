#pragma once

#include <vulkan/vulkan.h>

#include "RenderPass.h"
#include "renderer/DepthResources.h"

namespace narc_engine
{
    class ISurfaceProvider;
    class DeviceHandler;
    class Window;
    class Semaphore;

    class SwapChain : public narc_core::IGetter<VkSwapchainKHR>
    {
    public:
        SwapChain();
        ~SwapChain() override;

        NARC_IMPL_IGETTER(VkSwapchainKHR, m_swapChain)
        NARC_GETTER(const VkExtent2D&, getSwapChainExtent, m_swapChainExtent);
        NARC_GETTER(const VkFramebuffer&, getFrameBuffer, m_swapChainFramebuffers[imageIndex], const uint32_t& imageIndex);
        NARC_GETTER(const RenderPass*, getRenderPass, m_renderPass.get());

        void create(ISurfaceProvider* surface);
        void createFramebuffers();

        VkResult acquireNextImage(const Semaphore* semaphore, uint32_t* imageIndex);
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
