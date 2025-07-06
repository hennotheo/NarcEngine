//
// Created by theoh on 7/6/2025.
//
#include "test_device_base.h"
#include "backend_vulkan/queue/GraphicsQueueVulkan.h"
#include "backend_vulkan/queue/PresentQueueVulkan.h"

class VulkanQueueTest : public RhiDeviceTest
{
public:
    void SetUp() override
    {
        RhiDeviceTest::SetUp();

        EXPECT_NO_THROW(m_device->init()) << "DeviceRhi initialization threw an exception";
    }

    void TearDown() override
    {
        EXPECT_NO_THROW(m_device->shutdown()) << "DeviceRhi shutdown threw an exception";

        RhiDeviceTest::TearDown();
    }

protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(VulkanQueueTest, GraphicsQueueInitShutdown)
{
    const auto graphicsQueue = m_injector->create<GraphicsQueueRhiPtr>();

    EXPECT_NO_THROW(graphicsQueue->init()) << "Queue init failed";
    EXPECT_NO_THROW(graphicsQueue->shutdown()) << "Queue shutdown failed";
}

TEST_F(VulkanQueueTest, GraphicsQueueInitialisedQueueIsNotNull)
{
    const auto graphicsQueue = m_injector->create<GraphicsQueueRhiPtr>();
    const auto graphicsQueueVulkan = graphicsQueue->getGraphicsQueueVulkan();

    EXPECT_NO_THROW(graphicsQueueVulkan->init()) << "Queue init failed";

    EXPECT_NE(graphicsQueueVulkan->getQueue(), VK_NULL_HANDLE) << "Queue is not initialized";

    EXPECT_NO_THROW(graphicsQueueVulkan->shutdown()) << "Queue shutdown failed";
}

TEST_F(VulkanQueueTest, PresentQueueInitShutdown)
{
    const auto presentQueue = m_injector->create<PresentQueueRhiPtr>();

    EXPECT_NO_THROW(presentQueue->init()) << "Queue init failed";
    EXPECT_NO_THROW(presentQueue->shutdown()) << "Queue shutdown failed";
}

TEST_F(VulkanQueueTest, PresentQueueInitialisedQueueIsNotNull)
{
    const auto presentQueue = m_injector->create<PresentQueueRhiPtr>();
    const auto presentQueueVulkan = presentQueue->getPresentQueueVulkan();

    EXPECT_NO_THROW(presentQueueVulkan->init()) << "Queue init failed";

    EXPECT_NE(presentQueueVulkan->getQueue(), VK_NULL_HANDLE) << "Queue is not initialized";

    EXPECT_NO_THROW(presentQueueVulkan->shutdown()) << "Queue shutdown failed";
}