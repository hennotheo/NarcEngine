//
// Created by theoh on 27/05/2025.
//
#include "test_window_base.h"

class VulkanWindowTest : public RhiWindowTest
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(VulkanWindowTest, WindowRhi_Creation)
{
    const WindowRhiPtr window = createWindowRhi(getTestedApi(), m_context.get());
    ASSERT_NE(window.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API";
}

TEST_F(VulkanWindowTest, WindowRhi_InitShutdown)
{
    const WindowRhiPtr window = createWindowRhi(getTestedApi(), m_context.get());

    EXPECT_NO_THROW(window->init()) << "WindowRhi initialization threw an exception";
    EXPECT_NO_THROW(window->shutdown()) << "WindowRhi shutdown threw an exception";
}