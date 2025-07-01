//
// Created by theoh on 27/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"
#include "Rhi.h"

using namespace narc_engine;

class RhiWindowTest : public RhiTest
{
public:
    void SetUp() override
    {
        const auto injector = createDependencyInjector(getTestedApi());
        m_context = injector.create<ContextRhiPtr>();

        m_context->addExtension(RhiExtension::DebugUtils);
        m_context->addLayer(RhiLayer::Validation);
        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";

        m_context->init();
    }

    void TearDown() override
    {
        m_context->shutdown();

        m_context.reset();
    }

protected:
    ContextRhiPtr m_context;
};