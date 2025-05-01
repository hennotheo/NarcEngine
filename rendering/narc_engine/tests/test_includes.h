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
#include "core/EngineDebugLogger.h"
#include "core/devices/PhysicalDevice.h"
#include "core/devices/DeviceHandler.h"
#include "core/devices/LogicalDevice.h"

#include "core/queues/GraphicsQueue.h"
#include "core/queues/PresentQueue.h"

using namespace narc_engine;

#define INITIALISATION_TESTS(testName) TEST(NarcEngine_Initialisation, testName)
#define RUNTIME_TESTS(testName) TEST(NarcEngine_Runtime, testName)

const std::vector<const char*> g_validationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> g_deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
    VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
};

class MockEngine : public IEngineCallbacks
{
public:
    void stop() override {}
};