//
// Created by theohenno on 10/11/25.
//

#pragma once

namespace narc_engine {

    class VulkanInstance final : public narc_core::IInitialisable
    {
    public:
        VulkanInstance();
        ~VulkanInstance() override;

        NARC_IMPL_INITIALISABLE();

    private:
        VkInstance m_instance = VK_NULL_HANDLE;
    };

} // namespace narc_engine
