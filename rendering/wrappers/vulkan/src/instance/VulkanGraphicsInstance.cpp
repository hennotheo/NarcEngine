//
// Created by theo on 2/17/26.
//

#include "instance/VulkanGraphicsInstance.h"

#include "command/VulkanCommandPool.h"
#include "descriptor/VulkanDescriptorPool.h"
#include "descriptor/VulkanDescriptorSetLayout.h"
#include "instance/VulkanInstance.h"
#include "pipeline/VulkanGraphicsPipeline.h"
#include "pipeline/VulkanPipelineLayout.h"
#include "pipeline/VulkanRenderPass.h"
#include "services/VulkanMemoryAllocator.h"
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
        m_descriptorSetLayout = std::make_unique<VulkanDescriptorSetLayout>(m_device.get());
        m_descriptorSetLayout->addBinding({
                .BindingIndex = 0,
                .Stage = Vertex,
                .Type = UniformBuffer
        });
        m_descriptorSetLayout->addBinding({
                .BindingIndex = 1,
                .Stage = Fragment,
                .Type = Sampler
        });
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
        m_descriptorSetLayout->init();
    }

    void VulkanGraphicsInstance::shutdown()
    {
        m_descriptorSetLayout->shutdown();

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
        auto layout = std::make_unique<VulkanPipelineLayout>(m_device.get());
        layout->addDescriptorSetLayoutBinding(m_descriptorSetLayout.get());

        return layout;
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

    narc_core::result VulkanGraphicsInstance::waitForFences(const std::span<const IFence*> fences) const noexcept
    {
        return m_device->waitForFences(fences);
    }

    narc_core::result VulkanGraphicsInstance::resetFences(const std::span<const IFence*> fences) const noexcept
    {
        return m_device->resetFences(fences);
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
