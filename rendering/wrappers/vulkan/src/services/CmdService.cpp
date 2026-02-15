//
// Created by theo on 12/23/25.
//

#include "services/CmdService.h"

#include "command/VulkanCommandPool.h"
#include "device/VulkanDevice.h"

namespace narc_engine {
    CmdService::CmdService(NARC_DI_IMPORT_COMPONENT(VulkanDevice), NARC_DI_IMPORT_COMPONENT(VulkanCommandPool)) :
        NARC_DI_IMPL_COMPONENT(VulkanDevice, m_device),
        NARC_DI_IMPL_COMPONENT(VulkanCommandPool, m_commandPool)
    {
        //Empty constructor
    }

    CmdService::~CmdService() = default;

    narc_core::result CmdService::doCmdActionAndSubmit(const std::function<void(VulkanCommandBuffer&)> action) const noexcept
    {
        const auto cmd = m_commandPool->allocateOneTimeBuffer();
        cmd->begin();

        action(*cmd);

        cmd->end();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        const std::array commandBuffers = {cmd->getHandle()};
        submitInfo.commandBufferCount = commandBuffers.size();
        submitInfo.pCommandBuffers = commandBuffers.data();

        const auto graphicsQueue = m_device->getGraphicsQueue();
        if (graphicsQueue->submit(1, submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            NARC_LOG_FATAL("Submit command buffers failed.");
        }

        //TODO: Replace this later by a fence, vkQueueWaitIdle is overkill and high cost but fine in tests
        graphicsQueue->waitIdle();
        
        return true;
    }
} // narc_engine
