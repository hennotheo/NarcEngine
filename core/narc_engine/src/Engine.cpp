#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "data/Vertex.h"
#include "buffers/StagingBuffer.h"
#include "core/Window.h"

namespace narc_engine {
    static Engine* s_instance;

    Engine::Engine()
    {
        s_instance = this;
        m_window.init();
        createVulkanInstance();
        m_debugLogger.init(m_vulkanInstance);
        m_window.initSurface(m_vulkanInstance);
        m_deviceHandler.create(&m_window, m_vulkanInstance, m_debugLogger);
        m_commandPool.create(&m_deviceHandler);
        m_renderer.create();

        m_engineBinder = std::make_unique<EngineBinder>(this);
    }

    Engine::~Engine()
    {
        m_renderer.release();

        m_commandPool.release();
        m_deviceHandler.release();
        m_debugLogger.clean(m_vulkanInstance);

        m_window.cleanSurface(m_vulkanInstance);
        vkDestroyInstance(m_vulkanInstance, nullptr);

        m_window.clean();
    }

    Engine* Engine::getInstance()
    {
        return s_instance;
    }

    EngineBinder* Engine::binder()
    {
        return s_instance->m_engineBinder.get();
    }

    void Engine::createVulkanInstance()
    {
#ifdef ENABLE_VALIDATION_LAYERS
        if (!m_debugLogger.checkValidationLayerSupport())
        {
            throw std::runtime_error("Validation layers requested, but not available!");
        }
#endif

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Narcoleptic Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = m_debugLogger.getRequiredExtensions(m_window);
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        m_debugLogger.linkToInstance(createInfo, debugCreateInfo);

        if (vkCreateInstance(&createInfo, nullptr, &m_vulkanInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }
    }

    void Engine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        CommandBuffer commandBuffer = m_commandPool.beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        commandBuffer.cmdPipelineBarrier(sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        m_commandPool.endSingleTimeCommands(commandBuffer);
    }

    void Engine::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        CommandBuffer commandBuffer = m_commandPool.beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            width,
            height,
            1
        };

        commandBuffer.cmdCopyBufferImage(buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        m_commandPool.endSingleTimeCommands(commandBuffer);
    }

    void Engine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        CommandBuffer commandBuffer = m_commandPool.beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        commandBuffer.cmdCopyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

        m_commandPool.endSingleTimeCommands(commandBuffer);
    }

    void Engine::createImage(const narc_io::Image& imageData, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                             VkDeviceMemory& imageMemory) const
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = imageData.getWidth();
        imageInfo.extent.height = imageData.getHeight();
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional

        if (vkCreateImage(m_deviceHandler.getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_deviceHandler.getDevice(), image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = m_deviceHandler.findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_deviceHandler.getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(m_deviceHandler.getDevice(), image, imageMemory, 0);
    }
}
