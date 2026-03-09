//
// Created by theo on 3/5/26.
//

#pragma once

namespace narc_engine {
    class IDescriptorLayout : public narc_core::IInitialisable
    {
    public:
        NARC_PURE_VIRTUAL_SETTER(DescriptorSetBindingInfo, addBinding);
        NARC_PURE_VIRTUAL_GETTER(std::span<const DescriptorSetBindingInfo>, getBindings);
    };
}