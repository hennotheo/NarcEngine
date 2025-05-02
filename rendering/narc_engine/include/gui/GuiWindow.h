#pragma once

#include "gui/GuiComponent.h"

namespace narc_engine
{
    class NARC_ENGINE_API GuiWindow : public GuiComponent
    {
    public:
        GuiWindow(const std::string& name) : m_name(name) {}
        ~GuiWindow() override = default;

    protected:
        void onRender() const override final;

        virtual void renderContent() const { };

    private:
        std::string m_name;
    };
} // namespace narc_engine
