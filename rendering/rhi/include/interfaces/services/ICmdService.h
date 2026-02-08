//
// Created by theo on 12/23/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandBuffer;

    class ICmdService : public IService
    {
    public:
        virtual ~ICmdService() override = default;
        
        NARC_PURE_VIRTUAL_CMD(doCmdActionAndSubmit, std::function<void(VulkanCommandBuffer&)> action);
    };
}
