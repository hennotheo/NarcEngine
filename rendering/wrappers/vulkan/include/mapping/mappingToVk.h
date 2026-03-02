//
// Created by theo on 2/15/26.
//

#pragma once

namespace narc_engine::mapping {
    VkExtent2D mapFromExtend(const narc_math::Extent& extent) noexcept;

    VkApplicationInfo mapFromApplicationInfo(const ApplicationInfo& value) noexcept;

    VkSubmitInfo mapFromQueueSubmitInfos(const QueueSubmitInfos& value) noexcept;

    std::vector<VkFence_T*> toVkFenceArray(std::span<const IFence*> fences) noexcept;
}
