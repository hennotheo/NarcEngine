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
    /**
     * @test Verifies that a ContextRhi object can be successfully created for the Vulkan API.
     * @details Ensures that the created context is not null.
     */
    const DebuggerRhiPtr context = createDe(getTestedApi());
    ASSERT_NE(context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
}

TEST_F(RhiDebuggerTestVulkan, DebuggerRhi_InitShutdown)
{
    /**
     * @test Verifies that the ContextRhi object can be initialized and shut down without exceptions.
     * @details Ensures that the init() and shutdown() methods do not throw any exceptions.
     */
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    EXPECT_NO_THROW(context->init()) << "RhiDebugger initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "RhiDebugger shutdown threw an exception";
}