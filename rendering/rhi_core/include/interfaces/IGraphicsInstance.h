//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class IQueue;

    class IGraphicsInstance : public narc_core::IInitialisable
    {
    public:
        ~IGraphicsInstance() override = default;

        NARC_PURE_VIRTUAL_GETTER(const IQueue*, getGraphicsQueue);

        NARC_PURE_VIRTUAL_SETTER(ApplicationInfo, setApplicationInfo);
        NARC_PURE_VIRTUAL_SETTER(PhysicalDeviceCriteria, setDeviceCriteria);

        virtual void attachWindow(const IWindow* window) noexcept = 0;
    };
}
