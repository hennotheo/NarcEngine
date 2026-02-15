//
// Created by theo on 12/23/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandBuffer;

    class ICmdService : public narc_core::IService
    {
    public:
        ~ICmdService() override = default;
        
        NARC_PURE_VIRTUAL_CMD(doCmdActionAndSubmit, std::function<void(VulkanCommandBuffer&)> action);
    };
}
