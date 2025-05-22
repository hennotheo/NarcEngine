//
// Created by theoh on 3/18/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "rhi/ContextRhi.h"

using namespace narc_engine;

class RhiTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup code here
    }

    void TearDown() override
    {
        // Cleanup code here
    }

    virtual RendererApiType getTestedApi() = 0;
};

class RhiTestVulkan : public RhiTest
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(RhiTestVulkan, ContextRhi_Creation)
{
    ContextRhiPtr context = createContextRhi(getTestedApi());
    ASSERT_NE(context, nullptr) << "Failed to create ContextRhi for Vulkan API";
}

TEST_F(RhiTestVulkan, ContextRhi_InitShutdown)
{
    ContextRhiPtr context = createContextRhi(getTestedApi());
    EXPECT_NO_THROW(context->init()) << "ContextRhi initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "ContextRhi shutdown threw an exception";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
