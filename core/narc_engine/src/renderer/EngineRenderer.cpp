#include "renderer/EngineRenderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Engine.h"
#include "buffers/StaggingBuffer.h"

namespace narc_engine
{
    const uint32_t g_maxFramesInFlight = 2;

    void EngineRenderer::create()
    {
        m_device = Engine::getInstance()->getDevice()->getDevice();

        m_swapChain.create();
        createDescriptorSetLayout();
        m_renderTask.create(&m_swapChain, &m_descriptorSetLayout);
        m_swapChain.createFramebuffers(); // CreateFramebuffers();

        Engine::getInstance()->getCommandPool()->createCommandBuffers(g_maxFramesInFlight);

        createTextureImage();
        createImageTextureView();
        createTextureSampler();
        createUniformBuffers();
        m_renderTask.createDescriptorPool(g_maxFramesInFlight);
        m_renderTask.createDescriptorSets(g_maxFramesInFlight, m_descriptorSetLayout, m_uniformBuffers.data(), m_textureImageView, m_textureSampler);

        createSyncObjects();
    }

    void EngineRenderer::release()
    {
        m_swapChain.cleanSwapChain();

        vkDestroySampler(m_device, m_textureSampler, nullptr);
        vkDestroyImageView(m_device, m_textureImageView, nullptr);

        vkDestroyImage(m_device, m_textureImage, nullptr);
        vkFreeMemory(m_device, m_textureImageMemory, nullptr);

        for (UniformBuffer& buffer: m_uniformBuffers)
        {
            buffer.release();
        }

        m_renderTask.release();

        vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
        m_swapChain.cleanRenderPass();

        for (size_t i = 0; i < g_maxFramesInFlight; i++)
        {
            vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_device, m_imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
        }
    }

    void EngineRenderer::drawFrame()
    {
        vkWaitForFences(m_device, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = m_swapChain.acquireNextImage(m_imageAvailableSemaphores[m_currentFrame], &imageIndex);

        updateUniformBuffer(m_currentFrame);

        vkResetFences(m_device, 1, &m_inFlightFences[m_currentFrame]);

        CommandBuffer* buffer = Engine::getInstance()->getCommandPool()->getCommandBuffer(m_currentFrame);
        buffer->reset(0);
        recordCommandBuffer(buffer, imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] =
        {
            m_imageAvailableSemaphores[m_currentFrame]
        };
        VkPipelineStageFlags waitStages[] =
        {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffer->getVkCommandBuffer();

        VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (Engine::getInstance()->getDevice()->submitGraphicsQueue(1, &submitInfo, m_inFlightFences[m_currentFrame]) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {m_swapChain.getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        result = Engine::getInstance()->getDevice()->presentKHR(&presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Engine::getInstance()->getWindow()->
            isFramebufferResized())
        {
            Engine::getInstance()->getWindow()->setFramebufferResized(false);
            //after vkQueuePresentKHR to ensure that the semaphores are in a consistent state
            m_swapChain.reCreate();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        m_currentFrame = (m_currentFrame + 1) % g_maxFramesInFlight;
    }

    void EngineRenderer::updateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.Model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Proj = glm::perspective(glm::radians(45.0f), m_swapChain.getSwapChainExtent().width / (float) m_swapChain.getSwapChainExtent().height, 0.1f, 10.0f);
        ubo.Proj[1][1] *= -1;

        m_uniformBuffers[currentImage].setData(ubo);
    }

    void EngineRenderer::createDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    void EngineRenderer::recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        commandBuffer->begin(beginInfo);

        VkRenderPassBeginInfo renderPassInfo = m_swapChain.getRenderPassBeginInfos(imageIndex);
        VkExtent2D swapChainExtent = m_swapChain.getSwapChainExtent();

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        commandBuffer->cmdBeginRenderPass(&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        commandBuffer->cmdSetViewport(&viewport, 0, 1);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;
        commandBuffer->cmdSetScissor(&scissor, 0, 1);

        m_renderTask.recordTask(commandBuffer, m_currentFrame);
        commandBuffer->cmdEndRenderPass();

        if (commandBuffer->end() != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void EngineRenderer::createSyncObjects()
    {
        m_imageAvailableSemaphores.resize(g_maxFramesInFlight);
        m_renderFinishedSemaphores.resize(g_maxFramesInFlight);
        m_inFlightFences.resize(g_maxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < g_maxFramesInFlight; i++)
        {
            if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create semaphores!");
            }
        }
    }

    void EngineRenderer::createUniformBuffers()
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        m_uniformBuffers.resize(g_maxFramesInFlight);

        for (size_t i = 0; i < g_maxFramesInFlight; i++)
        {
            m_uniformBuffers[i].create(bufferSize);
        }
    }

    void EngineRenderer::createTextureImage()
    {
        narc_io::Image image = narc_io::FileReader::readImage("textures/logo.png");
        VkDeviceSize imageSize = image.getWidth() * image.getHeight() * 4;

        StaggingBuffer staggingBuffer;
        staggingBuffer.create(imageSize);
        staggingBuffer.input(image.getData());

        // stbi_image_free(pixels);

        Engine::getInstance()->createImage(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
                                           VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                           m_textureImage, m_textureImageMemory);

        Engine::getInstance()->transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
                                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Engine::getInstance()->copyBufferToImage(staggingBuffer.getBuffer(), m_textureImage,
                                                 static_cast<uint32_t>(image.getWidth()),
                                                 static_cast<uint32_t>(image.getHeight()));
        Engine::getInstance()->transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        staggingBuffer.release();
    }

    void EngineRenderer::createTextureSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        VkPhysicalDeviceProperties properties = Engine::getInstance()->getDevice()->getPhysicalDeviceProperties();
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(m_device, &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    void EngineRenderer::createImageTextureView()
    {
        m_textureImageView = m_swapChain.createImageView(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB);
    }
} // narc_engine
