//
// Created by theo on 3/5/26.
//

#pragma once

namespace narc_engine {
    class IDescriptorBinding
    {
    public:
        virtual ~IDescriptorBinding() = default;

        NARC_PURE_VIRTUAL_GETTER(const IDescriptorLayout*, getLayout);
    };
}