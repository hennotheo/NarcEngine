//
// Created by theohenno on 10/11/25.
//

#pragma once

namespace narc_engine {

    struct VulkanInstanceInfos
    {
        std::string ApplicationName;
        std::string EngineName;
    };

    class VulkanInstance final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanInstance(VulkanInstanceInfos creationInfos);
        ~VulkanInstance() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkInstance, getHandled, m_instance);

    private:
        VulkanInstanceInfos m_creationInfos;

        VkInstance m_instance = VK_NULL_HANDLE;
    };

} // namespace narc_engine
