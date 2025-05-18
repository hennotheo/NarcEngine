#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine
{
    class ShaderModule final : public narc_core::IGetter<VkShaderModule>
    {
    public:
        ShaderModule(const std::string& filename);
        ~ShaderModule();

        NARC_IMPL_IGETTER(VkShaderModule, m_shaderModule);

    private:
        VkShaderModule m_shaderModule;
    };
} // namespace narc_engine