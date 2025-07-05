//
// Created by theoh on 27/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"

using namespace narc_engine;

class RhiWindowTest : public RhiTest
{
public:
    void SetUp() override
    {
        auto injector = createRhiInjector(getTestedApi());

        m_context = injector.create<ContextRhiPtr>();
        m_window = injector.create<WindowRhiPtr>();

        m_context->addExtension(RhiExtension::DebugUtils);
        m_context->addLayer(RhiLayer::Validation);

        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
        ASSERT_NE(m_window.get(), nullptr) << "Failed to create WindowRhi for Vulkan API";

        m_context->init();
    }

    void TearDown() override
    {
        m_context->shutdown();

        m_context.reset();
        m_window.reset();
    }

protected:
    ContextRhiPtr m_context = nullptr;
    WindowRhiPtr m_window = nullptr;
};
