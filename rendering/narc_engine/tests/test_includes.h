//
// Created by theoh on 3/22/2025.
//

#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/interfaces/IEngineCallbacks.h"
#include "core/EngineBuilder.h"
#include "core/EngineInstance.h"
#include "core/Window.h"

using namespace narc_engine;

#define INITIALISATION_TESTS(testName) TEST(NarcEngine_Initialisation, testName)
#define RUNTIME_TESTS(testName) TEST(NarcEngine_Runtime, testName)

const std::vector<const char*> g_validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};