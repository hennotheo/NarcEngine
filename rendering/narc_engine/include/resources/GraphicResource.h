//
// Created by theoh on 09/03/2025.
//

#pragma once

#include "GraphicResourceHandler.h"
#include "core/DeviceComponent.h"

namespace narc_engine
{
    class DeviceHandler;

    class GraphicResource : public DeviceComponent
    {
    public:
        virtual ~GraphicResource();

        GETTER const GraphicResourceHandler& getResourceHandler() const { return m_resourceHandler; };

    protected:
        explicit GraphicResource(GraphicResourceType type, uint32_t resourceID);

    private:
        const GraphicResourceHandler m_resourceHandler;
    };
} // narc_engine
