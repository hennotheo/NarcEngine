//
// Created by theoh on 25/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"
#include "rhi/DeviceRhi.h"

using namespace narc_engine;

class RhiDeviceTest : public RhiTest
{
public:
    void SetUp() override
    {
        m_context = createContextRhi(getTestedApi());
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