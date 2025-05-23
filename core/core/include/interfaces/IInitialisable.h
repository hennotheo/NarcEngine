//
// Created by theoh on 5/23/2025.
//

#pragma once

namespace narc_core
{
#define NARC_IMPL_INITIALISABLE() \
void init() override; \
void shutdown() override

    class IInitialisable
    {
    public:
        virtual ~IInitialisable() = default;

        virtual void init() = 0;
        virtual void shutdown() = 0;
    };
} // namespace