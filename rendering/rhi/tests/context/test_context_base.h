//
// Created by theoh on 23/05/2025.
//

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test_rhi.h"

#include "rhi/ContextRhi.h"

class RhiExtensionsTest : public ::testing::TestWithParam<RhiExtension>{};
class RhiLayersTest : public ::testing::TestWithParam<RhiLayer>{};