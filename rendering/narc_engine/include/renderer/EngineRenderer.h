#pragma once

#include "CommandBuffer.h"
#include "DescriptorPool.h"
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

    public:
        EngineRenderer();
        ~EngineRenderer();

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);

        void attachRenderer(const Renderer* renderer);

    private:
        SwapChain m_swapChain;
        std::map<uint32_t, RenderTask*> m_rendererTasks;

        DescriptorPool m_descriptorPool;
        VkDescriptorSetLayout m_descriptorSetLayout;
        std::vector<VkDescriptorSet> m_descriptorSets;
        std::vector<UniformBuffer> m_uniformBuffers;
        uint32_t m_currentFrame = 0;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;

        VkImage m_textureImage;
        VkDeviceMemory m_textureImageMemory;
        VkImageView m_textureImageView;
        VkSampler m_textureSampler;

        const DeviceHandler* m_device = nullptr;

        void createDescriptorPool(uint32_t maxFrameInFlight);
        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        void createUniformBuffers();
        void createTextureImage(const narc_io::Image& sourceImage);
        void createTextureSampler();
        void createImageTextureView();
        RenderTask* createRenderTask(const Material* material);
    };
} // narc_engine
