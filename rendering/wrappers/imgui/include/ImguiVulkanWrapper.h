//
// Created by Theo on 5/9/2026.
//


#pragma once

#include <vulkan/vulkan.h>

struct ImGuiContext;

namespace narc_engine {

    class VulkanCommandBuffer;

    struct NarcImGuiContext
    {
        IGraphicsInstance* GraphicsInstance;
        IWindow* window;
        IGraphicsPipeline* Pipeline;

        uint32_t ImageCount;
        uint32_t MinImageCount;
    };


    class ImGuiBackend : public narc_core::IInitialisable
    {
    public:
        virtual void newFrame() = 0;
        virtual void endFrame() = 0;
    };

    class ImguiVulkanWrapper : public ImGuiBackend
    {
    public:
        explicit ImguiVulkanWrapper(const NarcImGuiContext& context);
        ~ImguiVulkanWrapper() override;

        NARC_IMPL_INITIALISABLE();

        void newFrame() override;
        void endFrame() override;
        void render(const VulkanCommandBuffer* cmdBuffer) const;
        void createDescriptorPool();

    private:

        ImGuiContext* m_context = nullptr;
        NarcImGuiContext m_vulkanContext{};

        VkDescriptorPool m_descriptorPool;
    };
}
