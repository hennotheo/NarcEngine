//
// Created by theohenno on 11/21/25.
//

#pragma once

#include "IVulkanSurface.h"

namespace narc_engine {
    class PhysicalDeviceService;
    class VulkanInstance;
    class VulkanSwapChain;

    class VulkanSurfacesManager : public narc_core::IInitialisable
    {
    public:
        explicit VulkanSurfacesManager(std::shared_ptr<narc_core::ICreator<VulkanSwapChain>> swapChainCreator);
        ~VulkanSurfacesManager() override;

        NARC_PURE_VIRTUAL_GETTER(const IVulkanSurface*, getMainSurface);
        NARC_IMPL_INITIALISABLE();

        virtual void updateSurfaces() = 0;

        std::vector<std::shared_ptr<IVulkanSurface>> getSurfaces() const noexcept { return m_surfaces; } //TODO: Temporary solution
        std::vector<VulkanSwapChain*> getSwapChains() const noexcept //TODO: Temporary solution
        {
            std::vector<VulkanSwapChain*> out;
            out.reserve(m_swapChains.size());
            std::transform(m_swapChains.begin(), m_swapChains.end(), std::back_inserter(out),
                           [](const std::unique_ptr<VulkanSwapChain>& swapChain) {
                               return swapChain.get();
                           });
            return out;
        }

        virtual void pushSurface(std::shared_ptr<IVulkanSurface>& surface);

    private:
        std::shared_ptr<narc_core::ICreator<VulkanSwapChain>> m_swapChainCreator;

        std::vector<std::shared_ptr<IVulkanSurface>> m_surfaces{};
        std::vector<std::unique_ptr<VulkanSwapChain>> m_swapChains{};
    };
} // narc_engine
