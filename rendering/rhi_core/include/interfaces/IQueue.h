//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class IQueue
    {
    public:
        virtual ~IQueue() = default;

        NARC_PURE_VIRTUAL_CMD(submit, QueueSubmitInfos infos);
        NARC_PURE_VIRTUAL_CMD(present, QueuePresentInfos infos);
    };
}