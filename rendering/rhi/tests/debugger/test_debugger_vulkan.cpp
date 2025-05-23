//
// Created by theoh on 5/23/2025.
//

#include "rhi/tests/test_rhi.h"
#include "test_debugger_base.h"

class RhiDebuggerTestVulkan : public RhiTest
{
public:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }

protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

TEST_F(RhiDebuggerTestVulkan, DebuggerRhi_Creation)
{
    const DebuggerRhiPtr debugger = createDebuggerRhi(getTestedApi());
    ASSERT_NE(debugger.get(), nullptr) << "Failed to create DebuggerRhiPtr for Vulkan API";
}

TEST_F(RhiDebuggerTestVulkan, DebuggerRhi_InitShutdown)
{
    const DebuggerRhiPtr context = createDebuggerRhi(getTestedApi());
    EXPECT_NO_THROW(context->init()) << "RhiDebugger initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "RhiDebugger shutdown threw an exception";
}