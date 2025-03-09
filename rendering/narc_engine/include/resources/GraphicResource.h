//
// Created by theoh on 09/03/2025.
//

#pragma once
#include "GraphicResourceHandler.h"

namespace narc_engine
{
    class DeviceHandler;

    class GraphicResource
    {
    public:
        virtual ~GraphicResource();

        [[nodiscard]] const GraphicResourceHandler& getResourceHandler() const { return m_resourceHandler; };

    protected:
        explicit GraphicResource(GraphicResourceType type, uint32_t resourceID);

        [[nodiscard]] const DeviceHandler* getDeviceHandler() const { return m_device; }

    private:
        const GraphicResourceHandler m_resourceHandler;

        const DeviceHandler* m_device = nullptr;
    };
} // narc_engine
