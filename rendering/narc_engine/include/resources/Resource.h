#pragma once

#include "resources/ResourceType.h"
#include "resources/ResourceState.h"

#define NARC_GET_RESOURCE_BY_ID(type, id) reinterpret_cast<type>(narc_engine::Resource::getResourceById(id));

#define NARC_RESOURCE_IMPLEMENTATION(type, ...) \
    friend class ResourceManager;\
    explicit type(__VA_ARGS__);\
    void onLoad() override;\
    void onUnload() override;\

namespace narc_engine
{
    class ResourceManager;

    class Resource : public narc_core::ILoadable
    {
        friend class ResourceManager;
    public:
        virtual ~Resource();

        NARC_GETTER(ResourceId, getId, m_id);
        NARC_GETTER(ResourceType, getType, m_type);

        NARC_IMPL_ILOADABLE();

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
