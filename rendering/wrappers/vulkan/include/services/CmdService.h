//
// Created by theo on 12/23/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandBuffer;
    class VulkanCommandPool;

    class CmdService : public ICmdService
    {
    public:
        explicit CmdService(NARC_DI_IMPORT_COMPONENT(VulkanDevice), NARC_DI_IMPORT_COMPONENT(VulkanCommandPool));
        ~CmdService() override;
        
        NARC_CMD_OVERRIDE(doCmdActionAndSubmit, std::function<void(VulkanCommandBuffer&)> action);
        
    private:
        narc_core::injected_component<VulkanDevice> m_device;
        narc_core::injected_component<VulkanCommandPool> m_commandPool;
    };
} // narc_engine
