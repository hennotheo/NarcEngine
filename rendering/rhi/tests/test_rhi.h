//
// Created by theoh on 5/23/2025.
//

#pragma once

#include <boost/di.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Rhi.h"

using namespace narc_engine;

class RhiTest : public ::testing::Test
{
protected:
    constexpr virtual RendererApiType getTestedApi() = 0;
};
