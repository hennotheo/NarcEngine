//
// Created by theohenno on 11/21/25.
//

#include "VulkanSurfacesManager.h"

#include <utility>

#include "../include/vulkan_wrappers/VulkanSwapChain.h"

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
    }

    void VulkanSurfacesManager::shutdown()
    {
        for (const auto& swapChains: m_swapChains)
        {
            swapChains->shutdown();
        }

        for (const auto& surface: m_surfaces)
        {
            surface->shutdown();
        }
    }

    void VulkanSurfacesManager::pushSurface(std::shared_ptr<IVulkanSurface>& surface)
    {
        auto localSwapChain = m_swapChainCreator->create();
        localSwapChain->setSurface(surface);

        m_surfaces.push_back(surface);
        m_swapChains.push_back(std::move(localSwapChain));
    }
}
