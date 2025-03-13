#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "DescriptorPool.h"
#include "MultiFrameManager.h"
#include "buffers/UniformBuffer.h"
#include "models/Image.h"
#include "models/Vertex.h"
#include "renderer/SwapChain.h"
#include "renderer/RenderTask.h"
#include "models/Mesh.h"
#include "models/Renderer.h"

namespace narc_engine {
    class Material;

    class EngineRenderer
    {
        friend class EngineBinder;
        friend class Engine;

    public:
        EngineRenderer();
        ~EngineRenderer();

        void drawFrame();
        void updateUniformBuffer(UniformBuffer* buffer) const;

        void attachRenderer(const Renderer* renderer);

    private:
        SwapChain m_swapChain;
        std::unique_ptr<MultiFrameManager> m_frameManager;
        std::map<uint32_t, RenderTask*> m_rendererTasks;

        // DescriptorPool m_descriptorPool;
        VkDescriptorSetLayout m_descriptorSetLayout;
        // std::vector<VkDescriptorSet> m_descriptorSets;
        // std::vector<UniformBuffer> m_uniformBuffers;

        // std::vector<VkSemaphore> m_imageAvailableSemaphores;
        // std::vector<VkSemaphore> m_renderFinishedSemaphores;
        // std::vector<VkFence> m_inFlightFences;

        // VkImage m_textureImage;
        // VkDeviceMemory m_textureImageMemory;
        // VkImageView m_textureImageView;
        // VkSampler m_textureSampler;

        const DeviceHandler* m_device = nullptr;

        void createDescriptorPool(uint32_t maxFrameInFlight);
        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex, const VkDescriptorSet* descriptorSet);
        void createSyncObjects();
        // void createUniformBuffers();
        // void createTextureImage(const narc_io::Image& sourceImage);
        // void createTextureSampler();
        // void createImageTextureView();
        RenderTask* createRenderTask(const Material* material);
    };
} // narc_engine
