#pragma once

#include "resources/ResourceType.h"
#include "resources/ResourceState.h"

#define NARC_GET_RESOURCE_BY_ID(type, id) reinterpret_cast<type>(narc_engine::Resource::getResourceById(id));

namespace narc_engine
{
    class ResourceManager;

    class Resource
    {
        friend class ResourceManager;

    public:
        virtual ~Resource();

        GETTER ResourceId getId() const { return m_id; };
        GETTER ResourceType getType() const { return m_type; };

        void load();
        void unload();

        QUERY static Resource* getResourceById(ResourceId id);

    protected:
        Resource(ResourceType type);

        virtual void onLoad() = 0;
        virtual void onUnload() = 0;

    _internal:
        void setId(ResourceId id) { m_id = id; }

    private:
        ResourceState m_state = ResourceState::Uninitialized;
        ResourceType m_type = ResourceType::Texture;

        ResourceId m_id;
    };
} // namespace narc_engine
