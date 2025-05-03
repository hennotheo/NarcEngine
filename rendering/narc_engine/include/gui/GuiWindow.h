#pragma once

#include "gui/GuiComponent.h"

namespace narc_gui
{
    class NARC_ENGINE_API GuiWindow : public GuiComponent
    {
    public:
        GuiWindow(const std::string& name) : m_name(name) {}
        ~GuiWindow() override = default;

        void setPosition(float x, float y) { m_position.x = x; m_position.y = y; }
        void setSize(float width, float height) { m_size.x = width; m_size.y = height; }

    protected:
        void onRender() const override final;

        virtual void renderContent() const {};

    private:
        std::string m_name;
        glm::vec2 m_position = { 0.0f, 0.0f };
        glm::vec2 m_size = { 0.0f, 0.0f };
    };
} // namespace narc_gui
