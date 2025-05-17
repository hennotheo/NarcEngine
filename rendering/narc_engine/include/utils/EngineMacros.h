#pragma once

#include "EngineFunctions.h"

#include "Engine.h"

#define NARC_COMMAND_POOL() Engine::getInstance()->getCommandPool()
#define NARC_BEGIN_SINGLE_TIME_COMMANDS() NARC_COMMAND_POOL()->beginSingleTimeCommands()
#define NARC_END_SINGLE_TIME_COMMANDS(commandBuffer) NARC_COMMAND_POOL()->endSingleTimeCommands(commandBuffer)

#define NARC_COPY_BUFFER(srcBuffer, dstBuffer, size) narc_engine::copyBuffer(srcBuffer, dstBuffer, size)