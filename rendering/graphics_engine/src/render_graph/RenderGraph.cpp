//
// Created by theo on 5/15/26.
//

#include "render_graph/RenderGraph.h"

namespace narc_engine {

    RenderGraph::RenderGraph(IGraphicsInstance* graphicsInstance) :
        m_graphicsInstance(graphicsInstance)
    {
    }

    RenderGraph::~RenderGraph()
    {
        for (const RenderGraphTextureResource& texture : m_textures)
        {
            if (texture.imported)
                continue;

            texture.OwnedImage->shutdown();
        }
    }

    RenderGraphTextureHandle RenderGraph::importTexture(std::string name, IImage* textureHandle)
    {
        RenderGraphTextureHandle handle;
        handle.index = static_cast<uint32_t>(m_textures.size());

        RenderGraphTextureResource resource;
        resource.name = std::move(name);
        resource.imported = true;
        resource.importedImage = textureHandle;

        m_textures.push_back(std::move(resource));

        return handle;
    }

    RenderGraphTextureHandle RenderGraph::createTexture(std::string name, const RenderGraphTextureDesc& desc)
    {
        RenderGraphTextureHandle handle;
        handle.index = static_cast<uint32_t>(m_textures.size());

        RenderGraphTextureResource resource;
        resource.name = std::move(name);
        resource.desc = desc;
        resource.imported = false;
        resource.importedImage = nullptr;

        m_textures.push_back(std::move(resource));

        return handle;
    }

    void RenderGraph::compile()
    {
        for (RenderGraphTextureResource& texture : m_textures)
        {
            if (texture.imported)
                continue;

            ImageAllocationInfo imageAllocationInfo;
            imageAllocationInfo.Extent = narc_math::Extent{ .Width = texture.desc.width, .Height = texture.desc.height };
            texture.OwnedImage = m_graphicsInstance->createImage(imageAllocationInfo);
            texture.OwnedImage->init();
        }
    }

    const IImage& RenderGraph::resolveImage(const RenderGraphTextureHandle handle) const
    {
        const RenderGraphTextureResource& resource = getTexture(handle);

        if (resource.imported)
        {
            assert(resource.importedImage != nullptr);
            return *resource.importedImage;
        }

        assert(resource.OwnedImage != nullptr);

        return *resource.OwnedImage;
    }

    const RenderGraphTextureResource& RenderGraph::getTexture(RenderGraphTextureHandle handle) const
    {
        assert(handle.isValid());
        assert(handle.index < m_textures.size());

        return m_textures[handle.index];
    }
}
