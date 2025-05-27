//
// Created by theoh on 25/05/2025.
//

#include "test_device_base.h"

#include "backend_vulkan/DeviceVulkan.h"

class VulkanDeviceTest : public RhiDeviceTest
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(VulkanDeviceTest, DeviceRhi_Creation)
{
    const DeviceRhiPtr device = createDeviceRhi(getTestedApi(), m_context.get(), m_window.get());
    ASSERT_NE(device.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API";
}

TEST_F(VulkanDeviceTest, DeviceRhi_InitShutdown)
{
    const DeviceRhiPtr device = createDeviceRhi(getTestedApi(), m_context.get(), m_window.get());

    EXPECT_NO_THROW(device->init()) << "DeviceRhi initialization threw an exception";
    EXPECT_NO_THROW(device->shutdown()) << "DeviceRhi shutdown threw an exception";
}

TEST_F(VulkanDeviceTest, DeviceRhi_GetDeviceVulkan)
{
    const DeviceRhiPtr device = createDeviceRhi(getTestedApi(), m_context.get(), m_window.get());

    const auto deviceVK = device->getDeviceVulkan();

    EXPECT_NE(deviceVK, nullptr);
}

TEST_F(VulkanDeviceTest, DeviceRhi_VkDeviceInitialized)
{
    const DeviceRhiPtr device = createDeviceRhi(getTestedApi(), m_context.get(), m_window.get());

    device->init();

    const auto deviceVK = device->getDeviceVulkan();
    EXPECT_NE(deviceVK, nullptr) << "DeviceVulkan is not implemented";
    EXPECT_NE(deviceVK->getVkDevice(), VK_NULL_HANDLE) << "VkDevice is not initialized";

    device->shutdown();
}