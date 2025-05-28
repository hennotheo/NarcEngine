//
// Created by theoh on 28/05/2025.
//
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"

#include "SwapChainRhi.h"
#include "backend_vulkan/SwapChainVulkan.h"

using namespace narc_engine;

class VulkanSwapChain : public RhiTest
{
public:
    void SetUp() override
    {
        m_context = createContextRhi(getTestedApi());
        m_window = createWindowRhi(getTestedApi(), m_context.get());
        m_device = createDeviceRhi(getTestedApi(), m_context.get(), m_window.get());

        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
        ASSERT_NE(m_window.get(), nullptr) << "Failed to create WindowRhi for Vulkan API";
        ASSERT_NE(m_device.get(), nullptr) << "Failed to create SwapChainRhi for Vulkan API";

        m_context->init();
        m_window->init();
        m_device->init();
    }

    void TearDown() override
    {
        m_device->shutdown();
        m_window->shutdown();
        m_context->shutdown();

        m_context.reset();
    }

protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }

protected:
    ContextRhiPtr m_context;
    WindowRhiPtr m_window;
    DeviceRhiPtr m_device;
};

TEST_F(VulkanSwapChain, Creation)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), m_window.get(), m_device.get());
    ASSERT_NE(swapChain.get(), nullptr) << "Failed to create SwapChainRhi for Vulkan API";
}


TEST_F(VulkanSwapChain, InitShutdown)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), m_window.get(), m_device.get());

    EXPECT_NO_THROW(swapChain->init()) << "DeviceRhi initialization threw an exception";
    EXPECT_NO_THROW(swapChain->shutdown()) << "DeviceRhi shutdown threw an exception";
}

TEST_F(VulkanSwapChain, GetSwapchainVulkan)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), m_window.get(), m_device.get());

    const auto swapChainVK = swapChain->getSwapChainVulkan();

    EXPECT_NE(swapChainVK, nullptr);
}

TEST_F(VulkanSwapChain, GetVkSwapchainKHRInitialized)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), m_window.get(), m_device.get());

    const auto swapChainVK = swapChain->getSwapChainVulkan();

    swapChain->init();
    VkSwapchainKHR vkSwapChain = swapChainVK->getVkSwapChain();
    swapChain->shutdown();

    EXPECT_NE(vkSwapChain, VK_NULL_HANDLE);
}