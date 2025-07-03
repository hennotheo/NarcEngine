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

class DeviceVulkanExtensionTest
    : public VulkanDeviceTest, public ::testing::WithParamInterface<const char*>
{
};

TEST_F(VulkanDeviceTest, Creation)
{
    ASSERT_NE(m_device.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API";
}

TEST_F(VulkanDeviceTest, InitShutdown)
{
    EXPECT_NO_THROW(m_device->init()) << "DeviceRhi initialization threw an exception";
    EXPECT_NO_THROW(m_device->shutdown()) << "DeviceRhi shutdown threw an exception";
}

TEST_F(VulkanDeviceTest, GetDeviceVulkan)
{
    const auto deviceVK = m_device->getDeviceVulkan();

    EXPECT_NE(deviceVK, nullptr);
}

TEST_F(VulkanDeviceTest, PhysicalDeviceVulkanCreation)
{
    EXPECT_NO_THROW(PhysicalDeviceVulkan physicalDevice(m_context->getContextVulkan()););
}

TEST_P(DeviceVulkanExtensionTest, QueryPhysicalDeviceWithExtension)
{
    PhysicalDeviceVulkan physicalDevice(m_context->getContextVulkan());
    physicalDevice.addExtension(GetParam());
    EXPECT_NO_THROW(physicalDevice.queryPhysicalDevice());
}

INSTANTIATE_TEST_SUITE_P(
    VulkanDeviceTest,
    DeviceVulkanExtensionTest,
    ::testing::Values(
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_MAINTENANCE1_EXTENSION_NAME,
        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
    )
    );

TEST_F(VulkanDeviceTest, VkDeviceInitialized)
{
    m_device->init();

    const auto deviceVK = m_device->getDeviceVulkan();
    EXPECT_NE(deviceVK, nullptr) << "DeviceVulkan is not implemented";
    EXPECT_NE(deviceVK->getVkDevice(), VK_NULL_HANDLE) << "VkDevice is not initialized";

    m_device->shutdown();
}
