#include "resources/ResourceManager.h"

namespace narc_engine
{
    constexpr uint32_t BASE_ALLOCATED_RESOURCES = 20;

    ResourceManager::ResourceManager()
    {
        m_resources.reserve(BASE_ALLOCATED_RESOURCES);
        m_allocationOrder.reserve(BASE_ALLOCATED_RESOURCES);
    }

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