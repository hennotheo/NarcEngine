//
// Created by theoh on 25/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"

using namespace narc_engine;

class RhiDeviceTest : public RhiTest
{
public:
    void SetUp() override
    {
        m_injector = std::make_unique<RhiInjector>(createDependencyInjector(getTestedApi()));

        m_context = m_injector->create<ContextRhiPtr>();
        m_window = m_injector->create<WindowRhiPtr>();

        m_device = m_injector->create<DeviceRhiPtr>();

        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
        ASSERT_NE(m_window.get(), nullptr) << "Failed to create WindowRhi for Vulkan API";

        m_context->init();
        m_window->init();

        int t = 0;
    }

    void TearDown() override
    {
        m_window->shutdown();
        m_context->shutdown();

        m_injector.reset();
    }

protected:
    ContextRhiPtr m_context;
    WindowRhiPtr m_window;

    DeviceRhiPtr m_device;

    std::unique_ptr<RhiInjector> m_injector;
};
