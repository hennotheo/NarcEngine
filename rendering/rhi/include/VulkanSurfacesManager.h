//
// Created by theohenno on 11/21/25.
//

#pragma once
#include "vulkan_wrappers/VulkanGraphicsPipeline.h"

namespace narc_engine {
    class VulkanFramebuffer;
    class VulkanDevice;
    class IVulkanSurface;
    class VulkanGraphicsPipeline;
    class DeviceService;
    class VulkanInstance;
    class VulkanSwapChain;
    
    struct SurfaceComponentReferences
    {
        VulkanPipelineLayout* Layout;
    };

    class VulkanSurfacesManager : public ISurfacesHandler, public narc_core::IInitialisable
    {
    public:
        explicit VulkanSurfacesManager(std::shared_ptr<narc_core::ICreator<VulkanSwapChain>> swapChainCreator);
        ~VulkanSurfacesManager() override;
        
        NARC_IMPL_INITIALISABLE();

        void setDevice(std::weak_ptr<VulkanDevice> device) noexcept
        {
            m_device = std::move(device);
        }

        virtual void updateSurfaces() = 0;

        std::vector<IVulkanSurface*> getSurfaces() const noexcept
        {
            auto out = std::vector<IVulkanSurface*>{};
            out.reserve(m_surfaces.size());
            std::transform(m_surfaces.begin(), m_surfaces.end(), std::back_inserter(out),
                           [](const std::unique_ptr<IVulkanSurface>& surface) {
                               return surface.get();
                           });
            return out;
        } //TODO: Temporary solution
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

        const std::vector<std::vector<std::unique_ptr<VulkanFramebuffer>>>& getFramebuffers() const noexcept
        {            
            return m_frameBuffers;
        }

        std::vector<VulkanGraphicsPipeline*> getPipeline() const noexcept
        {
            std::vector<VulkanGraphicsPipeline*> out;
            out.reserve(m_pipelines.size());
            for (const auto& pipeline: m_pipelines)
            {
                    out.push_back(pipeline.get());
            }
            
            return out;
        }

        virtual SurfaceComponentReferences pushSurface(std::unique_ptr<IVulkanSurface>& surface);

    protected:
        NARC_GETTER(const std::weak_ptr<VulkanDevice>&, getDevice, m_device);

    private:
        std::weak_ptr<VulkanDevice> m_device;
        std::shared_ptr<narc_core::ICreator<VulkanSwapChain>> m_swapChainCreator;

        std::vector<std::unique_ptr<IVulkanSurface>> m_surfaces{};
        std::vector<std::unique_ptr<VulkanGraphicsPipeline>> m_pipelines{};
        std::vector<std::unique_ptr<VulkanSwapChain>> m_swapChains{};
        std::vector<std::vector<std::unique_ptr<VulkanFramebuffer>>> m_frameBuffers{};
    };
} // narc_engine
