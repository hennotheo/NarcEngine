//
// Created by theoh on 09/03/2025.
//

#pragma once
#include "resources/GraphicResourceHandler.h"

namespace narc_engine
{
    struct GraphicResourceHandler;

    class GraphicResource;

    class NARC_ENGINE_API EngineResourcesManager final
    {
    public:
        const GraphicResourceHandler& createTexture2D(const char* path);

        [[nodiscard]] const GraphicResource* getResource(const GraphicResourceHandler& resourceHandler) const;
    private:
        uint32_t m_resourceIDCounter = 0;
        std::map<uint32_t, GraphicResource*> m_resources;
    };
} // narc_engine
