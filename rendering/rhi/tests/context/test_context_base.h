//
// Created by theoh on 23/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"

#include "rhi/ContextRhi.h"


struct ExtensionParam
{
    std::string name;
    const std::vector<RhiExtension> extensions;
};

class RhiExtensionsTest : public ::testing::TestWithParam<ExtensionParam>{};
class RhiLayersTest : public ::testing::TestWithParam<RhiLayer>{};