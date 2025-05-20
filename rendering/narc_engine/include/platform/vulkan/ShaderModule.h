#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine
{
    class ShaderModule final : public narc_core::IGetter<VkShaderModule>, public narc_core::ILoadable
    {
    public:
        ShaderModule(const char* filename);
        ~ShaderModule();

        NARC_IMPL_IGETTER(VkShaderModule, m_shaderModule);
        NARC_IMPL_ILOADABLE();

    private:
        const char* m_filename = nullptr;

        VkShaderModule m_shaderModule = VK_NULL_HANDLE;
    };
} // namespace narc_engine