//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class IQueue
    {
    public:
        virtual ~IQueue() = default;

        NARC_PURE_VIRTUAL_GETTER(QueueFamilyIndex, getQueueFamilyIndex);
        NARC_PURE_VIRTUAL_CMD(submit, QueueSubmitInfos infos);
        NO_DISCARD virtual QueuePresentResult present(QueuePresentInfos infos) const noexcept = 0;
        NARC_PURE_VIRTUAL_CMD(waitQueueIdle);
    };
}