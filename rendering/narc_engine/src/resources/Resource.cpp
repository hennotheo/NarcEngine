#include "resources/Resource.h"

namespace narc_engine
{
    Resource::Resource(ResourceType type) : m_type(type), m_id(0)
    {
        
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


} // namespace narc_engine
