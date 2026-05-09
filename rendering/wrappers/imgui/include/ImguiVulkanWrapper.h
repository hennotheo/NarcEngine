//
// Created by Theo on 5/9/2026.
//


#pragma once

struct ImGuiContext;

#include <vulkan/vulkan.h>

namespace narc_engine {
    struct NarcImGuiContext
    {
        IGraphicsInstance* GraphicsInstance;
        IWindow* window;
        IGraphicsPipeline* Pipeline;

        uint32_t ImageCount;
        uint32_t MinImageCount;
    };

    class ImguiVulkanWrapper : public narc_core::IInitialisable
    {
    public:
        explicit ImguiVulkanWrapper(const NarcImGuiContext& context);
        ~ImguiVulkanWrapper() override;

        NARC_IMPL_INITIALISABLE();

        void newFrame();
        void endFrame();
        void render(const ICommandBuffer* cmdBuffer);
        void createDescriptorPool();

    private:
        ImGuiContext* m_context = nullptr;
        NarcImGuiContext m_vulkanContext{};

        VkDescriptorPool m_descriptorPool;
    };
}
