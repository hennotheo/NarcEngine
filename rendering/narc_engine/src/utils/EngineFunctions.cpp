#include "utils/EngineFunctions.h"

#include "buffers/Buffer.h"
#include "platform/vulkan/CommandBuffer.h"
#include "platform/vulkan/CommandPoolRhi.h"

#include "utils/EngineMacros.h"

#include "Engine.h"
#include "core/devices/LogicalDevice.h"
#include "core/devices/PhysicalDevice.h"

namespace narc_engine
{
    void copyBuffer(const Buffer* srcBuffer, const Buffer* dstBuffer, DeviceSize size)
    {
        CommandBuffer commandBuffer = NARC_BEGIN_SINGLE_TIME_COMMANDS();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        commandBuffer.cmdCopyBuffer(srcBuffer->get(), dstBuffer->get(), 1, &copyRegion);

        NARC_END_SINGLE_TIME_COMMANDS(commandBuffer);
    }

    const CommandPoolRhi* getCommandPool()
    {
        return Engine::getInstance()->getCommandPool();
    }
    
    const PhysicalDevice* getPhysicalDevice()
    {
        return Engine::getInstance()->getDevice()->getPhysicalDevice();
    }

    const LogicalDevice* getLogicalDevice()
    {
        return Engine::getInstance()->getDevice()->getLogicalDevice();
    }
} // namespace narc_engine
