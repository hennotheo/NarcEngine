#pragma once

#include "resources/Resource.h"

namespace narc_engine
{
    class ResourceManager
    {
        using ResourceCounter = uint32_t;

    public:
        ResourceManager();
        ~ResourceManager();

        template <typename T, typename... Args>
        inline ResourceId createResource(Args&&... args)
        {
            static_assert(std::is_base_of<Resource, T>::value, "T must be derived from Resource");
            static_assert(!std::is_same<T, Resource>::value, "T must not be Resource itself");

            const char* typeName = typeid(T).name();
            if (!typeName)
            {
                NARCLOG_FATAL("typeid(T).name() returned null");
            }

            std::hash<std::string> hasher;
            std::string combinedArgs = (std::string(typeName)) + std::to_string(m_resourceCounter);;
            ResourceId id = std::to_string(hasher(combinedArgs));
            
            auto ptr = new T(std::forward<Args>(args)...);
            m_resources[id] = std::unique_ptr<T>(ptr);
            m_resources[id]->setId(id);
            m_resources[id]->load();
            m_allocationOrder.push_back(id);

            m_resourceCounter++;

            return id;
        }

        inline Resource* getResource(ResourceId id)
        {
            auto it = m_resources.find(id);
            if (it != m_resources.end())
            {
                return it->second.get();
            }
            return nullptr;
        }

        void destroyResource(ResourceId id);

    private:
        std::unordered_map<ResourceId, std::unique_ptr<Resource>> m_resources;
        std::vector<ResourceId> m_allocationOrder;

        TEMP_CODE ResourceCounter m_resourceCounter = 0;
    };
} // namespace narc_engine