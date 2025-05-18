#pragma once

#include "EngineFunctions.h"

#include "core/devices/LogicalDevice.h"
#include "core/devices/PhysicalDevice.h"

#define NARC_DEVICE narc_engine::getLogicalDevice()
#define NARC_DEVICE_HANDLE narc_engine::getLogicalDevice()->get()

#define NARC_PHYSICAL_DEVICE narc_engine::getPhysicalDevice()
#define NARC_PHYSICAL_DEVICE_HANDLE narc_engine::getPhysicalDevice()->get()

#define NARC_COMMAND_POOL narc_engine::getCommandPool()
#define NARC_BEGIN_SINGLE_TIME_COMMANDS() NARC_COMMAND_POOL->beginSingleTimeCommands()
#define NARC_END_SINGLE_TIME_COMMANDS(commandBuffer) NARC_COMMAND_POOL->endSingleTimeCommands(commandBuffer)

#define NARC_COPY_BUFFER(srcBuffer, dstBuffer, size) narc_engine::copyBuffer(srcBuffer, dstBuffer, size)