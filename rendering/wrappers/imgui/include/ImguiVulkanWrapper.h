//
// Created by Theo on 5/9/2026.
//


#pragma once

#include <vulkan/vulkan.h>

struct ImGuiContext;

namespace narc_engine {
    struct GuiInitContext;

    class VulkanCommandBuffer;


    class ImGuiBackend : public narc_core::IInitialisable
    {
    public:
        virtual void newFrame() = 0;
        virtual void endFrame() = 0;
    };

    class ImguiVulkanWrapper : public ImGuiBackend
    {
    public:
        explicit ImguiVulkanWrapper(const GuiInitContext& context);
        ~ImguiVulkanWrapper() override;

        NARC_IMPL_INITIALISABLE();

        void newFrame() override;
        void endFrame() override;
        void render(const VulkanCommandBuffer* cmdBuffer) const;
        void createDescriptorPool();

    private:

        ImGuiContext* m_context = nullptr;
        GuiInitContext m_vulkanContext{};

        VkDescriptorPool m_descriptorPool;
    };
}
