#include "resources/ResourceManager.h"

namespace narc_engine
{
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