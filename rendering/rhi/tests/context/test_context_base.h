//
// Created by theoh on 23/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rhi/ContextRhi.h"

using namespace narc_engine;

class RhiTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Setup code here
    }

    void TearDown() override
    {
        // Cleanup code here
    }

    virtual RendererApiType getTestedApi() = 0;
};

class RhiExtensionsTest : public ::testing::TestWithParam<RhiExtension>{};
class RhiLayersTest : public ::testing::TestWithParam<RhiLayer>{};