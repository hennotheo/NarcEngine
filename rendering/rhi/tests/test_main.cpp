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

class RhiExtensionsTest : public ::testing::TestWithParam<RhiExtensions>
{
};

TEST_P(RhiExtensionsTest, Extensions_Enabled)
{
    const RhiExtensions extension = GetParam();
    const ContextRhiPtr context = createContextRhi(RendererApiType::Vulkan);

    context->addExtensions(&extension, 1);

    EXPECT_TRUE(context->isExtensionEnabled(extension));
}

TEST_P(RhiExtensionsTest, Extensions_InitShutdown)
{
    const RhiExtensions extension = GetParam();
    const ContextRhiPtr context = createContextRhi(RendererApiType::Vulkan);

    context->addExtensions(&extension, 1);

    EXPECT_NO_THROW(context->init()) << "ContextRhi initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "ContextRhi shutdown threw an exception";
}

INSTANTIATE_TEST_SUITE_P(RhiExtensionsVulkanTest, RhiExtensionsTest,
                         ::testing::Values(RhiExtensions::Core, RhiExtensions::DebugUtils, RhiExtensions::Surface,
                                           RhiExtensions::ExtendedDevicesProperties, RhiExtensions::ExtendedSurfaceCapabilities));

class RhiTestVulkan : public RhiTest
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(RhiTestVulkan, ContextRhi_Creation)
{
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    ASSERT_NE(context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
}

TEST_F(RhiTestVulkan, ContextRhi_InitShutdown)
{
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    EXPECT_NO_THROW(context->init()) << "ContextRhi initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "ContextRhi shutdown threw an exception";
}

TEST_F(RhiTestVulkan, ContextRhi_Core_Enabled)
{
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    EXPECT_TRUE(context->isExtensionEnabled(RhiExtensions::Core));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
