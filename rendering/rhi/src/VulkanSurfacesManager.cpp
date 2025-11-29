//
// Created by theohenno on 11/21/25.
//

#include "VulkanSurfacesManager.h"

#include "IVulkanSurface.h"
#include "vulkan_wrappers/VulkanGraphicsPipeline.h"
#include "vulkan_wrappers/VulkanPipelineLayout.h"
#include "vulkan_wrappers/VulkanRenderPass.h"
#include "vulkan_wrappers/VulkanSwapChain.h"

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

        for (const auto& swapChains: m_swapChains)
        {
            swapChains->init();
        }

        for (const auto& pipelines: m_pipelines)
        {
            pipelines->init();
        }
    }

    void VulkanSurfacesManager::shutdown()
    {
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
