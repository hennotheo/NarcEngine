//
// Created by theo on 5/10/26.
//

#pragma once

namespace narc_engine {
    class IGui : public narc_core::IInitialisable
    {
    public:
        ~IGui() override = default;

        virtual void newFrame() = 0;
        virtual void startWindow(const std::string& name) = 0;
        virtual void text(const std::string& content) = 0;
        virtual void spacing() = 0;
        virtual void endWindow() = 0;
        virtual void endFrame() = 0;
        virtual void render(const ICommandBuffer* cmdBuffer) = 0;
    };
}
