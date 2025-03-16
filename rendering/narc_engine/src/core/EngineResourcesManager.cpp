//
// Created by theoh on 09/03/2025.
//

#include "core/EngineResourcesManager.h"

#include "NarcLog.h"
#include "resources/Texture2DResource.h"

namespace narc_engine
{
    EngineResourcesManager::~EngineResourcesManager()
    {
        for (auto [id, resource] : m_resources)
        {
            delete resource;
            resource = nullptr;
        }
    }

    const GraphicResourceHandler& EngineResourcesManager::createTexture2D(const char* path)
    {
        Texture2DResource* texture = new Texture2DResource(path, m_resourceIDCounter);
        m_resources.emplace(m_resourceIDCounter, texture);

        m_resourceIDCounter++;

        return texture->getResourceHandler();
    }

    const GraphicResource* EngineResourcesManager::getResource(const GraphicResourceHandler& resourceHandler) const
    {
        if (!m_resources.contains(resourceHandler.ResourceID))
        {
            NARCLOG_FATAL(std::string("Resource with ID: {} does not exist!", resourceHandler.ResourceID));
        }

        return m_resources.at(resourceHandler.ResourceID);
    }
} // narc_engine
