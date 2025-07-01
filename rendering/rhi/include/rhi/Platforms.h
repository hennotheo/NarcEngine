#pragma once

#define NARC_FATAL_OPENGL_NOT_SUPPORTED() NARCLOG_FATAL("OpenGL is not supported yet")
#define NARC_FATAL_DIRECTX12_NOT_SUPPORTED() NARCLOG_FATAL("DirectX12 is not supported yet")

#define NARC_DECL_RHI_PLATFORM_GETTER(className, platform) NARC_VIRTUAL_GETTER(className##platform*, get ## className##platform, nullptr)

#define NARC_DECL_RHI_PLATFORM_TYPES(className) \
    class className##Vulkan;

#define NARC_DECL_RHI_PLATFORM_GETTERS(className) \
    NARC_DECL_RHI_PLATFORM_GETTER(className, Vulkan)

#define NARC_IMPL_RHI_PLATFORM_GETTER(className, platform) NARC_OVERRIDE_GETTER(className##platform*, get ## className##platform, const_cast<className##platform*>(this))

#define NARC_IMPL_VK_PLATFORM_GETTERS(className) NARC_IMPL_RHI_PLATFORM_GETTER(className, Vulkan)

namespace narc_engine
{
    enum class RendererApiType
    {
        Vulkan,
        OpenGL,
        DirectX12,
    };
} // namespace narc_engine
