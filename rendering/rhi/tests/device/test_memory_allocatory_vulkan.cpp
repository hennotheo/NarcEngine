//
// Created by theoh on 7/6/2025.
//

#include "test_device_base.h"

class VulkanMemoryAllocatorTest : public RhiDeviceTest
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

TEST_F(VulkanMemoryAllocatorTest, MemoryAllocatorCreation)
{
    EXPECT_NO_THROW(m_memoryAllocator->init()) << "MemoryAllocatorRhi initialization threw an exception";
    EXPECT_NO_THROW(m_memoryAllocator->shutdown()) << "MemoryAllocatorRhi shutdown threw an exception";
}