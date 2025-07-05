//
// Created by theoh on 28/05/2025.
//
#include "test_swapchain_base.h"

using namespace narc_engine;

class VulkanSwapChain : public RhiSwapChain
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};


TEST_F(VulkanSwapChain, InitShutdown)
{
    EXPECT_NO_THROW(m_swapChain->init()) << "DeviceRhi initialization threw an exception";
    EXPECT_NO_THROW(m_swapChain->shutdown()) << "DeviceRhi shutdown threw an exception";
}

TEST_F(VulkanSwapChain, GetSwapchainVulkan)
{
    const auto swapChainVK = m_swapChain->getSwapChainVulkan();

    EXPECT_NE(swapChainVK, VK_NULL_HANDLE);
}

TEST_F(VulkanSwapChain, GetVkSwapchainKHRInitialized)
{
    const auto swapChainVK = m_swapChain->getSwapChainVulkan();

    m_swapChain->init();
    const VkSwapchainKHR vkSwapChain = swapChainVK->getVkSwapChain();
    m_swapChain->shutdown();

    EXPECT_NE(vkSwapChain, VK_NULL_HANDLE);
}
