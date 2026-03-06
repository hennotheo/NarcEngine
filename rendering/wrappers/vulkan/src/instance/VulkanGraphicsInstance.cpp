//
// Created by theo on 2/17/26.
//

#include "instance/VulkanGraphicsInstance.h"

#include "VulkanTextureImage.h"
#include "command/VulkanCommandPool.h"
#include "descriptor/VulkanDescriptorPool.h"
#include "descriptor/VulkanDescriptorSetLayout.h"
#include "descriptor/VulkanDescriptorSet.h"
#include "instance/VulkanInstance.h"
#include "pipeline/VulkanGraphicsPipeline.h"
#include "pipeline/VulkanPipelineLayout.h"
#include "pipeline/VulkanRenderPass.h"
#include "surface/VulkanLinuxSurface.h"
#include "swapchain/VulkanSwapChain.h"
#include "sync/VulkanFence.h"
#include "sync/VulkanSemaphore.h"

namespace narc_engine {
    VulkanGraphicsInstance::VulkanGraphicsInstance() :
        m_mainWindow(nullptr)
    {
        m_instance = std::make_unique<VulkanInstance>();
        m_device = std::make_unique<VulkanDevice>(m_instance.get());
        m_descriptorPool = std::make_unique<VulkanDescriptorPool>(m_device.get());
    }

    VulkanGraphicsInstance::~VulkanGraphicsInstance() noexcept = default;

    void VulkanGraphicsInstance::init()
    {
        m_instance->init();

        const auto surface = createSurface(m_mainWindow);
        surface->init();
        m_device->setMainWindowSurface(static_cast<IVulkanSurface*>(surface.get()));
        m_device->init();
        surface->shutdown();

        m_descriptorPool->setDescriptorCount(2); //TODO: Change hardcoded value
        m_descriptorPool->init();
    }

    void VulkanGraphicsInstance::shutdown()
    {
        m_descriptorPool->shutdown();
        m_device->shutdown();
        m_instance->shutdown();
    }

    void VulkanGraphicsInstance::setApplicationInfo(const ApplicationInfo& value) noexcept
    {
        m_instance->setApplicationInfo(value);
    }

    void VulkanGraphicsInstance::setDeviceCriteria(const PhysicalDeviceCriteria& value) noexcept
    {
        m_device->setPhysicalDeviceCriteria(value);
    }

    std::unique_ptr<ISwapchain> VulkanGraphicsInstance::createSwapChain(const ISurface* surface) const noexcept
    {
        auto swapchain = std::make_unique<VulkanSwapChain>(m_device.get(), dynamic_cast<const IVulkanSurface*>(surface), 2);//TODO: 2 HARDCODED

        m_swapChainRenderPasses.emplace(std::make_pair(swapchain.get(), std::make_unique<VulkanRenderPass>(m_device.get(), swapchain.get())));

        swapchain->setRenderPass(m_swapChainRenderPasses[swapchain.get()].get());

        return swapchain;
    }

    std::unique_ptr<IPipelineLayout> VulkanGraphicsInstance::createPipelineLayout(const ISwapchain* swapChain) const noexcept
    {
        return std::make_unique<VulkanPipelineLayout>(m_device.get());
    }

    std::unique_ptr<IGraphicsPipeline> VulkanGraphicsInstance::createPipeline(const IPipelineLayout* layout,
                                                                              const ISwapchain* swapChain) const noexcept
    {
        auto renderPass = std::make_unique<VulkanRenderPass>(m_device.get(), static_cast<const VulkanSwapChain*>(swapChain));

        return std::make_unique<VulkanGraphicsPipeline>(m_device.get(),
                                                        static_cast<const VulkanSwapChain*>(swapChain),
                                                        static_cast<const VulkanPipelineLayout*>(layout),
                                                        renderPass);
    }

    std::unique_ptr<ISemaphore> VulkanGraphicsInstance::createSemaphore() const noexcept
    {
        return std::make_unique<VulkanSemaphore>(m_device.get());
    }

    std::unique_ptr<IFence> VulkanGraphicsInstance::createFence() const noexcept
    {
        return std::make_unique<VulkanFence>(m_device.get());
    }

    std::unique_ptr<ICommandBufferPool> VulkanGraphicsInstance::createCommandBufferPool() const noexcept
    {
        return std::make_unique<VulkanCommandPool>(m_device.get());
    }

    std::unique_ptr<IBuffer> VulkanGraphicsInstance::createBuffer(const BufferAllocationInfo& allocationInfo) const noexcept
    {
        return m_device->createBuffer(allocationInfo).value_or(nullptr);
    }

    std::unique_ptr<IImage> VulkanGraphicsInstance::createImage(const ImageAllocationInfo& allocationInfo) const noexcept
    {
        return m_device->createImage(allocationInfo).value_or(nullptr);
    }

    std::unique_ptr<IDescriptorLayout> VulkanGraphicsInstance::createDescriptorLayout() const noexcept
    {
        return std::make_unique<VulkanDescriptorSetLayout>(m_device.get());
    }

    std::vector<std::unique_ptr<IDescriptorBinding>> VulkanGraphicsInstance::createDescriptorBinding(const IDescriptorLayout* layout) const noexcept
    {
        const auto vkLayout = narc_core::backend_cast<VulkanDescriptorSetLayout>(layout);

        std::vector layouts {vkLayout, vkLayout};//TODO: TEMP pour alloc plusieurs Bindings pour les frame in flight
        return m_descriptorPool->allocateDescriptorSet(layouts);
    }

    narc_core::result VulkanGraphicsInstance::waitForFences(const std::span<const IFence*> fences) const noexcept
    {
        return m_device->waitForFences(fences);
    }

    narc_core::result VulkanGraphicsInstance::resetFences(const std::span<const IFence*> fences) const noexcept
    {
        return m_device->resetFences(fences);
    }

    narc_core::result VulkanGraphicsInstance::waitIdle() const noexcept
    {
        m_device->waitIdle();
        return true;
    }

    std::unique_ptr<ISurface> VulkanGraphicsInstance::createSurface(const IWindow* window) const noexcept
    {
        return std::make_unique<VulkanLinuxSurface>(m_instance.get(), window);
    }

    void VulkanGraphicsInstance::attachWindow(const IWindow* window) noexcept
    {
        m_mainWindow = window;
    }
}
