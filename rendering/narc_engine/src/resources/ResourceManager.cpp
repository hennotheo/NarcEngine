#include "resources/ResourceManager.h"

namespace narc_engine
{
    ResourceManager::~ResourceManager()
    {
        for (ResourceId& resourceID : m_allocationOrder)
        {
            m_resources[resourceID]->unload();
        }

        m_resources.clear();
        m_allocationOrder.clear();
    }

    void ResourceManager::destroyResource(ResourceId id)
    {
        auto it = m_resources.find(id);
        if (it != m_resources.end())
        {
            it->second->unload();
            m_resources.erase(it);
        }
    }
} // namespace narc_engine