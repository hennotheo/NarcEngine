#include "resources/Resource.h"

#include "Engine.h"

namespace narc_engine
{
    Resource::Resource(ResourceType type) : m_type(type), m_id("")
    {
        
    }

    Resource::~Resource()
    {
        if(m_state == ResourceState::Loaded)
        {
            unload();
        }
    }

    void Resource::load()
    {
        if (m_state == ResourceState::Loaded)
        {
            NARCLOG_WARNING("Resource is already loaded");
            return;
        }

        m_state = ResourceState::Loaded;
        onLoad();
    }

    void Resource::unload()
    {
        if (m_state != ResourceState::Loaded)
        {
            NARCLOG_WARNING("Resource is not loaded");
            return;
        }

        m_state = ResourceState::Unloaded;
        onUnload();
    }

    Resource* Resource::getResourceById(ResourceId id)
    {
        return Engine::getInstance()->resourceManager()->getResource(id);
    }
} // namespace narc_engine
