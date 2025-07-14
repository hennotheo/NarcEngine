//
// Created by theoh on 27/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <GLFW/glfw3.h>

#include "test_rhi.h"

using namespace narc_engine;

class RhiWindowTest : public RhiTest
{
public:
    void SetUp() override
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        const auto injector = createRhiInjector(getTestedApi());

        m_context = injector.create<ContextRhiPtr>();
        m_device = injector.create<DeviceRhiPtr>();
        m_window = injector.create<WindowRhiPtr>();

        m_context->addExtension(RhiExtension::DebugUtils);
        m_context->addLayer(RhiLayer::Validation);

        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
        ASSERT_NE(m_device.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API";
        ASSERT_NE(m_window.get(), nullptr) << "Failed to create WindowRhi for Vulkan API";

        ASSERT_NO_THROW(m_context->init()) << "ContextRhi initialization threw an exception";
        ASSERT_NO_THROW(m_device->init()) << "DeviceRhi initialization threw an exception";
    }

    void TearDown() override
    {
        m_device->shutdown();
        m_context->shutdown();

        m_context.reset();
        m_window.reset();
    }

protected:
    ContextRhiPtr m_context = nullptr;
    DeviceRhiPtr m_device = nullptr;
    WindowRhiPtr m_window = nullptr;
};
