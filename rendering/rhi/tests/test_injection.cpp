//
// Created by theoh on 7/12/2025.
//

#include "test_rhi.h"


TEST(InjectionTest, CreateVulkanInjector)
{
    ASSERT_NO_THROW(createRhiInjector(RendererApiType::Vulkan));
}

TEST(InjectionTest, InjectContext)
{
    const auto injector = createRhiInjector(RendererApiType::Vulkan);

    ASSERT_NO_THROW(injector.create<ContextRhiPtr>());
}

TEST(InjectionTest, InjectDevice)
{
    auto injector = createRhiInjector(RendererApiType::Vulkan);

    ASSERT_NO_THROW(injector.create<DeviceRhiPtr>());
}

TEST(InjectionTest, InjectWindow)
{
    auto injector = createRhiInjector(RendererApiType::Vulkan);

    ASSERT_NO_THROW(injector.create<WindowRhiPtr>());
}