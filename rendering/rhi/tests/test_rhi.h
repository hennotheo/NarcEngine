//
// Created by theoh on 5/23/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace narc_engine;

class RhiTest : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    virtual RendererApiType getTestedApi() = 0;
};