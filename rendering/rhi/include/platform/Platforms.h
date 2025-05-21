#pragma once

#define NARC_FATAL_OPENGL_NOT_SUPPORTED() NARCLOG_FATAL("OpenGL is not supported yet")
#define NARC_FATAL_DIRECTX12_NOT_SUPPORTED() NARCLOG_FATAL("DirectX12 is not supported yet")

namespace narc_engine
{
    enum class RendererApiType
    {
        Vulkan,
        OpenGL,
        DirectX12,
    };
} // namespace narc_engine
