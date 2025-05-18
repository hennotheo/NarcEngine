//
// Created by theoh on 3/2/2025.
//

#include "renderer/DepthResources.h"

#include "Engine.h"

namespace narc_engine {
    DepthResources::~DepthResources()
    {
    }

    void DepthResources::create(uint32_t width, uint32_t height)
    {
        const VkFormat depthFormat = NARC_PHYSICAL_DEVICE->findDepthFormat();

        Engine::getInstance()->createImage(
            width, height,
            depthFormat, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, &m_depthImageMemory);
        m_depthImageView.create(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        Engine::getInstance()->transitionImageLayout(m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }

    void DepthResources::release()
    {
        m_depthImageView.release();
        vkDestroyImage(NARC_DEVICE_HANDLE, m_depthImage, nullptr);
        m_depthImageMemory.release();
    }
}
