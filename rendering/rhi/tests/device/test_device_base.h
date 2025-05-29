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
        m_context = createContextRhi(getTestedApi());
        m_window = createWindowRhi(getTestedApi(), m_context.get());

        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
        ASSERT_NE(m_window.get(), nullptr) << "Failed to create WindowRhi for Vulkan API";

        m_context->init();
        m_window->init();
    }

    void TearDown() override
    {
        m_window->shutdown();
        m_context->shutdown();

        m_context.reset();
    }

protected:
    ContextRhiPtr m_context;
    WindowRhiPtr m_window;
};