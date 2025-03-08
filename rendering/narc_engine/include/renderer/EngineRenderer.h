#pragma once

#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "buffers/UniformBuffer.h"
#include "models/Image.h"
#include "models/Vertex.h"
#include "renderer/SwapChain.h"
#include "renderer/RenderTask.h"
#include "models/Mesh.h"

namespace narc_engine {
    class EngineRenderer
    {
        friend class EngineBinder;

    public:
        EngineRenderer(const narc_io::Image& sourceImage);
        ~EngineRenderer();

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);

        void bindMesh(const Mesh* mesh) { m_renderTask.bindMesh(mesh); }
        void unbindMesh(const Mesh* mesh) { m_renderTask.unbindMesh(mesh); }

        void bindImage(const narc_io::Image& image);

    private:
        SwapChain m_swapChain;
        RenderTask m_renderTask;

        DescriptorPool m_descriptorPool;
        VkDescriptorSetLayout m_descriptorSetLayout;
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
    };
} // narc_engine
