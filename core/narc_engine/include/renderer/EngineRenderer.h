#pragma once

#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "buffers/GraphicsBuffer.h"
#include "buffers/UniformBuffer.h"
#include "data/Image.h"
#include "data/Vertex.h"
#include "renderer/SwapChain.h"
#include "renderer/RenderTask.h"

namespace narc_engine
{
    class EngineRenderer
    {
    public:
        void create();

        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);

        void release();

    private:
        SwapChain m_swapChain;
        RenderTask m_renderTask;

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

        VkDevice m_device;

        void createDescriptorPool();
        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex);
        void createSyncObjects();
        void createUniformBuffers();
        void createTextureImage();
        void createTextureSampler();
        void createImageTextureView();
    };
} // narc_engine
