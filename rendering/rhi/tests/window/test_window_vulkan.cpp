//
// Created by theoh on 27/05/2025.
//
#include "test_window_base.h"

class VulkanWindowTest : public RhiWindowTest
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(VulkanWindowTest, WindowRhi_Creation) { ASSERT_NE(m_window.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API"; }

TEST_F(VulkanWindowTest, WindowRhi_InitShutdown)
{
    EXPECT_NO_THROW(m_window->init()) << "WindowRhi initialization threw an exception";
    EXPECT_NO_THROW(m_window->shutdown()) << "WindowRhi shutdown threw an exception";
}

TEST_F(VulkanWindowTest, WindowRhi_InitGLFWwindow)
{
    m_window->init();

    EXPECT_NE(m_window->m_window, nullptr) << "Failed to create GLFW window";

    m_window->shutdown();
}

TEST_F(VulkanWindowTest, WindowRhi_DestroyGLFWwindow)
{
    m_window->init();
    m_window->shutdown();

    EXPECT_EQ(m_window->m_window, nullptr) << "GLFW window was not destroyed properly during shutdown";
}

TEST_F(VulkanWindowTest, WindowRhi_InitCreateSurface)
{
    m_window->init();

    EXPECT_NE(m_window->getWindowVulkan()->getVkSurface(), nullptr) << "Failed to create Vulkan surface for the GLFW window";

    m_window->shutdown();
}
