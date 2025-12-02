//
// Created by theohenno on 11/21/25.
//

#include "VulkanSurfacesManager.h"

#include "IVulkanSurface.h"
#include "vulkan_wrappers/VulkanGraphicsPipeline.h"
#include "vulkan_wrappers/VulkanPipelineLayout.h"
#include "vulkan_wrappers/VulkanRenderPass.h"
#include "vulkan_wrappers/VulkanSwapChain.h"
#include "vulkan_wrappers/VulkanFramebuffer.h"

namespace narc_engine {
    VulkanSurfacesManager::VulkanSurfacesManager(std::shared_ptr<narc_core::ICreator<VulkanSwapChain>> swapChainCreator) :
        m_swapChainCreator(std::move(swapChainCreator))
    {
    }

    VulkanSurfacesManager::~VulkanSurfacesManager() = default;

    void VulkanSurfacesManager::init()
    {
        for (const auto& surface: m_surfaces)
        {
            surface->init();
        }

        m_frameBuffers.clear();
        m_frameBuffers.resize(m_swapChains.size());
        for (int i = 0; i < m_swapChains.size(); ++i)
        {
            const auto& swapChains = m_swapChains[i];
            const auto& pipeline = m_pipelines[i];
            const auto* renderPass = pipeline->getRenderPass();

            //Need to init swap chain before framebuffer creation
            swapChains->init();
            pipeline->init();
            
            auto* framebufferArray = &m_frameBuffers[i];
            const auto& imageViews = swapChains->getSwapChainImageViews();
            framebufferArray->reserve(imageViews.size());
            for (const auto& imageView: imageViews)
            {
                std::vector attachments = {imageView};

                auto framebuffer = std::make_unique<VulkanFramebuffer>(m_device, swapChains.get(), renderPass);
                framebuffer->setAttachments(attachments);
                
                framebufferArray->push_back(std::move(framebuffer));
            }
        }

        for (const auto& framebuffers: m_frameBuffers)
        {
            for (const auto& framebuffer: framebuffers)
            {
                framebuffer->init();
            }
        }
    }

    void VulkanSurfacesManager::shutdown()
    {
        for (const auto& framebuffers: m_frameBuffers)
        {
            for (const auto& framebuffer: framebuffers)
            {
                framebuffer->shutdown();
            }
        }

        for (const auto& pipelines: m_pipelines)
        {
            pipelines->shutdown();
        }

        for (const auto& swapChains: m_swapChains)
        {
            swapChains->shutdown();
        }

        for (const auto& surface: m_surfaces)
        {
            surface->shutdown();
        }
    }

    void VulkanSurfacesManager::pushSurface(std::unique_ptr<IVulkanSurface>& surface)
    {
        auto localSwapChain = m_swapChainCreator->create();
        localSwapChain->setSurface(surface);

        auto pipeline = std::make_unique<VulkanGraphicsPipeline>(m_device, localSwapChain);
        auto renderPass = std::make_unique<VulkanRenderPass>(localSwapChain, m_device);
        auto pipelineLayout = std::make_unique<VulkanPipelineLayout>(m_device);
        pipeline->setLayout(pipelineLayout);
        pipeline->setRenderPass(renderPass);

        m_pipelines.push_back(std::move(pipeline));
        m_surfaces.push_back(std::move(surface));
        m_swapChains.push_back(std::move(localSwapChain));
    }
}
