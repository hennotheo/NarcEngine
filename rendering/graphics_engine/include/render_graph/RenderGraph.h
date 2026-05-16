//
// Created by theo on 5/15/26.
//


#pragma once

namespace narc_engine {
    enum class TextureFormat
    {
        RGBA8,
        RGBA16F,
        Depth24Stencil8,
        Depth32F
    };

    enum class TextureUsageFlags : uint32_t
    {
        None            = 0,
        ColorAttachment = 1 << 0,
        DepthAttachment = 1 << 1,
        ShaderRead      = 1 << 2,
        Storage         = 1 << 3,
        TransferSrc     = 1 << 4,
        TransferDst     = 1 << 5,
        Present         = 1 << 6
    };

    struct RenderGraphTextureDesc
    {
        uint32_t width = 0;
        uint32_t height = 0;

        TextureFormat format = TextureFormat::RGBA8;
        TextureUsageFlags usage = TextureUsageFlags::None;

        uint32_t mipLevels = 1;
        uint32_t arrayLayers = 1;
        uint32_t sampleCount = 1;
    };

    struct RenderGraphTextureHandle
    {
        uint32_t index = UINT32_MAX;

        bool isValid() const { return index != UINT32_MAX; }
    };

    struct RenderGraphTextureResource
    {
        std::string name;
        RenderGraphTextureDesc desc;

        IImage* importedImage = nullptr;
        std::unique_ptr<IImage> OwnedImage;

        bool imported = false;
    };

    class RenderGraph
    {
    public:
        explicit RenderGraph(IGraphicsInstance* graphicsInstance);
        ~RenderGraph();

        RenderGraphTextureHandle importTexture(std::string name, IImage* textureHandle);
        RenderGraphTextureHandle createTexture(std::string name, const RenderGraphTextureDesc& desc);
        void compile();

        const IImage& resolveImage(RenderGraphTextureHandle handle) const;
    private:
        IGraphicsInstance* m_graphicsInstance = nullptr;

        std::vector<RenderGraphTextureResource> m_textures;

        const RenderGraphTextureResource& getTexture(RenderGraphTextureHandle handle) const;
    };
}