#include "Engine.h"

#include "models/Vertex.h"
#include "buffers/StagingBuffer.h"
#include "core/Window.h"

namespace narc_engine {
    static Engine* s_instance;

    IEngine* getEngine()
    {
#ifdef NARC_ENGINE_PLATFORM_WINDOWS
        return Engine::getInstance();
#endif
        return nullptr;
    }

    IEngine* createEngine()
    {
#ifdef NARC_ENGINE_PLATFORM_WINDOWS
        return new Engine();
#endif
        NARCLOG_FATAL("Engine not implemented for this platform!");
    }

    Engine::Engine()
    {
        s_instance = this;
        m_window = std::make_unique<Window>();
        m_instance = std::make_unique<EngineInstance>();
        m_window->init(m_instance.get());

        if (m_instance == nullptr || m_window == nullptr)
        {
            NARCLOG_FATAL("Failed to initialize Engine: m_instance or m_window is null");
        }

        m_debugLogger = std::make_unique<EngineDebugLogger>(m_instance.get());
        m_deviceHandler = std::make_unique<DeviceHandler>(m_window.get(), m_instance.get(), m_debugLogger.get());

        m_commandPool = std::make_unique<CommandPool>();
        m_renderer = std::make_unique<EngineRenderer>();

        m_engineBinder = std::make_unique<EngineBinder>(this);
        m_resourcesManager = std::make_unique<EngineResourcesManager>();
    }

    Engine::~Engine() = default;

    Engine* Engine::getInstance()
    {
        return s_instance;
    }

    EngineBinder* Engine::binder() const
    {
        return m_engineBinder.get();
    }

    EngineResourcesManager* Engine::resourceManager() const
    {
        return m_resourcesManager.get();
    }

    void Engine::pollEvents()
    {
        m_window->update();
    }

    bool Engine::shouldClose() const
    {
        return m_window->shouldClose();
    }

    void Engine::render()
    {
        m_renderer->drawFrame();
    }

    void Engine::waitDeviceIdle()
    {
        m_deviceHandler->waitDeviceIdle();
    }

    bool Engine::hasStencilComponent(VkFormat format)
    {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void Engine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        CommandBuffer commandBuffer = getCommandPool()->beginSingleTimeCommands();

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

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (hasStencilComponent(format))
            {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        //Set Source access masks
        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout ==
                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else
        {
            NARCLOG_FATAL("Unsupported layout transition!");
        }

        commandBuffer.cmdPipelineBarrier(sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        getCommandPool()->endSingleTimeCommands(commandBuffer);
    }

    void Engine::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        CommandBuffer commandBuffer = getCommandPool()->beginSingleTimeCommands();

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

        getCommandPool()->endSingleTimeCommands(commandBuffer);
    }

    void Engine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        CommandBuffer commandBuffer = getCommandPool()->beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        commandBuffer.cmdCopyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

        getCommandPool()->endSingleTimeCommands(commandBuffer);
    }

    void Engine::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                             VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                             VkDeviceMemory& imageMemory) const
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
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

        if (vkCreateImage(m_deviceHandler->getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_deviceHandler->getDevice(), image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = m_deviceHandler->findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_deviceHandler->getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to allocate image memory!");
        }

        vkBindImageMemory(m_deviceHandler->getDevice(), image, imageMemory, 0);
    }

    void Engine::createImage(const narc_io::Image& imageData, VkFormat format, VkImageTiling tiling,
                             VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                             VkDeviceMemory& imageMemory) const
    {
        createImage(imageData.getWidth(),
                    imageData.getHeight(),
                    format,
                    tiling,
                    usage,
                    properties,
                    image,
                    imageMemory);
    }
}
