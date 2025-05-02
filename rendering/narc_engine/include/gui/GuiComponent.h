#pragma once

#include "interfaces/IGuiHandle.h"

namespace narc_engine
{
    class NARC_ENGINE_API GuiComponent : public IGuiHandle
    {
    public:
        GuiComponent() = default;
        virtual ~GuiComponent() = default;

        void show() override;
        void hide() override;

    _internal:
        void render() const override final;

    protected:
        virtual void onRender() const = 0;

    private:
        bool m_visible = true;
    };
} // namespace narc_engine