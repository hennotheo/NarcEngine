#pragma once

namespace narc_engine
{
    class NARC_ENGINE_API IGuiHandle
    {
    public:
        virtual ~IGuiHandle() = default;

        virtual void show() = 0;
        virtual void hide() = 0;

    _internal_protected:
        virtual void render() const = 0;
    };
} // namespace narc_engine
