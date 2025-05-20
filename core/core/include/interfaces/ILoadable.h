#pragma once 

namespace narc_core
{
#define NARC_IMPL_ILOADABLE() \
        void load() override; \
        void unload() override

    class ILoadable
    {
    public:
        virtual ~ILoadable() = default;

        virtual void load() = 0;
        virtual void unload() = 0;
    };
} // namespace 