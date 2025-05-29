//
// Created by theoh on 29/05/2025.
//
#include "test_rhi.h"

class DependencyInjectionTest : public RhiTest
{
public:
    void SetUp() override
    {
        m_context = createContextRhi(getTestedApi());
        m_window = createWindowRhi(getTestedApi(), m_context.get());
        m_device = createDeviceRhi(getTestedApi(), m_context.get(), m_window.get());
    }

    void TearDown() override
    {
        m_device.reset();
        m_window.reset();
        m_context.reset();
    }

protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }

    ContextRhiPtr m_context;
    WindowRhiPtr m_window;
    DeviceRhiPtr m_device;
};

TEST_F(DependencyInjectionTest, Context_InjectNoThrow)
{
    EXPECT_NO_THROW(m_context->injectDeviceDependency(m_device));
    EXPECT_NO_THROW(m_context->injectWindowDependency(m_window));
}

TEST_F(DependencyInjectionTest, Context_DestroyItDestroyDependency)
{
    m_context->injectDeviceDependency(m_device);

    DeviceRhiPtr& devicePtr = m_context->m_device;
    EXPECT_NE(devicePtr, nullptr);
    EXPECT_NE(devicePtr.get(), nullptr);

    m_context.reset();

    EXPECT_EQ(devicePtr, nullptr);
    EXPECT_EQ(devicePtr.get(), nullptr);
}