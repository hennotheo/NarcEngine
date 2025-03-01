#pragma once

#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "buffers/UniformBuffer.h"
#include "data/Vertex.h"
#include "renderer/SwapChain.h"
#include "renderer/RenderTask.h"
#include "data/Mesh.h"

namespace narc_engine {
    class EngineRenderer
    {
    public:
        void create();

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);

        void bindMesh(const Mesh* mesh) { m_renderTask.bindMesh(mesh); }
        void unbindMesh(const Mesh* mesh) { m_renderTask.unbindMesh(mesh); }

        void release();

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

        VkImage m_depthImage;
        VkDeviceMemory m_depthImageMemory;
        VkImageView m_depthImageView;

        VkDevice m_device;

        void createDescriptorPool(uint32_t maxFrameInFlight);
        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        void createUniformBuffers();
        void createDepthResources();
        void createTextureImage();
        void createTextureSampler();
        void createImageTextureView();
    };
} // narc_engine
