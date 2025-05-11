#pragma once

#include "resources/ResourceType.h"
#include "resources/ResourceState.h"

namespace narc_engine
{
    class Resource
    {
    public:
        Resource(ResourceType type);
        virtual ~Resource() = default;

        GETTER ResourceId getId() const { return m_id; };
        GETTER ResourceType getType() const { return m_type; };

        void load();
        void unload();

    protected:
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
