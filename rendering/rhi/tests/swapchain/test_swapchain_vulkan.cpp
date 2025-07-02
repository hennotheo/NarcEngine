//
// Created by theoh on 28/05/2025.
//
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"

#include "ContextRhi.h"
#include "SwapChainRhi.h"
#include "backend_vulkan/SwapChainVulkan.h"

using namespace narc_engine;

class VulkanSwapChain : public RhiTest
{
public:
    void SetUp() override
    {
        const auto injector = createDependencyInjector(getTestedApi());
        m_context = injector.create<ContextRhiPtr>();

        m_window = injector.create<WindowRhiPtr>();
        m_device = injector.create<DeviceRhiPtr>();

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
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), *m_window, *m_device);
    ASSERT_NE(swapChain.get(), nullptr) << "Failed to create SwapChainRhi for Vulkan API";
}


TEST_F(VulkanSwapChain, InitShutdown)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), *m_window, *m_device);

    EXPECT_NO_THROW(swapChain->init()) << "DeviceRhi initialization threw an exception";
    EXPECT_NO_THROW(swapChain->shutdown()) << "DeviceRhi shutdown threw an exception";
}

TEST_F(VulkanSwapChain, GetSwapchainVulkan)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), *m_window, *m_device);

    const auto swapChainVK = swapChain->getSwapChainVulkan();

    EXPECT_NE(swapChainVK, VK_NULL_HANDLE);
}

TEST_F(VulkanSwapChain, GetVkSwapchainKHRInitialized)
{
    const SwapChainRhiPtr swapChain = createSwapChainRhi(getTestedApi(), *m_window, *m_device);

    const auto swapChainVK = swapChain->getSwapChainVulkan();

    swapChain->init();
    const VkSwapchainKHR vkSwapChain = swapChainVK->getVkSwapChain();
    swapChain->shutdown();

    EXPECT_NE(vkSwapChain, VK_NULL_HANDLE);
}
