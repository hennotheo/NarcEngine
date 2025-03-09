//
// Created by theoh on 09/03/2025.
//

#include "resources/GraphicResource.h"

#include "Engine.h"

namespace narc_engine
{
    GraphicResource::GraphicResource(const GraphicResourceType type, const uint32_t resourceID): m_resourceHandler(resourceID, type)
    {
        m_device = Engine::getInstance()->getDevice();
    }

    GraphicResource::~GraphicResource()
    {
    }
} // narc_engine
