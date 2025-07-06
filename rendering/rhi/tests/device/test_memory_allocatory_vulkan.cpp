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

        m_device->init();
    }

    void TearDown() override
    {
        m_device->shutdown();

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