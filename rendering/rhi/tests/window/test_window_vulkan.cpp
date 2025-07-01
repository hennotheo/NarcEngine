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
    ASSERT_NE(m_window.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API";
}

TEST_F(VulkanWindowTest, WindowRhi_InitShutdown)
{
    EXPECT_NO_THROW(m_window->init()) << "WindowRhi initialization threw an exception";
    EXPECT_NO_THROW(m_window->shutdown()) << "WindowRhi shutdown threw an exception";
}