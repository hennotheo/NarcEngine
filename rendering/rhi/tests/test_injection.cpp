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
    auto injector = boost::di::make_injector(
        boost::di::bind<ContextRhi>.to<ContextVulkan>().in(boost::di::singleton));

    ASSERT_NO_THROW(injector.create<ContextVulkan>());
}

TEST(InjectionTest, InjectDevice)
{
    auto injector = createRhiInjector(RendererApiType::Vulkan);

    ASSERT_NO_THROW(injector.create<DeviceVulkan>());
}

TEST(InjectionTest, InjectWindow)
{
    auto injector = createRhiInjector(RendererApiType::Vulkan);

    using InjectedType = decltype(injector.create<WindowVulkan>());

    static_assert(std::is_same<InjectedType, WindowVulkan>::value,
                  "WindowVulkan is not injectable");

    // const std::weak_ptr<ContextRhi> ctx = injector.create<ContextRhiPtr>();
    // const std::weak_ptr<DeviceRhi> dev = injector.create<DeviceRhiPtr>();
    auto ctx = injector.create<ContextRhiPtr>();
    auto dev = injector.create<DeviceRhiPtr>();

    ASSERT_NO_THROW(injector.create<WindowRhiPtr>());
}

TEST(InjectionTest, InjectSwapChain)
{
    auto injector = createRhiInjector(RendererApiType::Vulkan);

    ASSERT_NO_THROW(injector.create<SwapChainRhiPtr>());
}
