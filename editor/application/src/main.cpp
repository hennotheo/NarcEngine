#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

#include "models/Vertex.h"

class SurfaceManager final : public narc_engine::VulkanSurfacesManager
{
public:
    explicit SurfaceManager(std::shared_ptr<narc_core::ICreator<narc_engine::VulkanSwapChain> > swapChainCreator) :
        VulkanSurfacesManager(std::move(swapChainCreator))
    {
    }

    ~SurfaceManager() override = default;

    NO_DISCARD const narc_engine::IVulkanSurface* getMainSurface() const noexcept override
    {
        const auto m_surfaces = getSurfaces();
        return m_surfaces.empty() ? nullptr : m_surfaces.front();
    }

    narc_engine::VulkanSemaphore* imageAvailableSemaphore;
    narc_engine::VulkanSemaphore* renderFinishedSemaphore;
    narc_engine::VulkanFence* inFlightFence;
    narc_engine::VulkanCommandBuffer* cmdBuffer;
    narc_engine::VulkanQueue* graphicsQueue;
    narc_engine::VulkanQueue* presentQueue;
    narc_engine::VulkanVertexBuffer* vertexBuffer;

    void updateSurfaces() override
    {
        NARC_GUARD_WEAK(device, getDevice(), "Failed to get Vulkan Device.");

        const auto surfaces = getSurfaces();
        const auto swapChains = getSwapChains();
        const auto& framebuffers = getFramebuffers();
        const auto pipelines = getPipeline();
        for (int i = 0; i < surfaces.size(); ++i)
        {
            const auto surf = surfaces[i];
            const auto swapchain = swapChains[i];
            const auto& swapchainFBs = framebuffers[i];
            const auto pipeline = pipelines[i];

            inFlightFence->wait();
            inFlightFence->reset();

            uint32_t imageIndex;
            vkAcquireNextImageKHR(device->getHandle(), swapchain->getHandle(), UINT64_MAX, imageAvailableSemaphore->getHandle(), VK_NULL_HANDLE,
                                  &imageIndex);

            const auto& framebuffer = swapchainFBs[imageIndex];
            cmdBuffer->reset();
            recordCommandBuffer(framebuffer.get(), swapchain, pipeline);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            std::array waitSemaphores = {imageAvailableSemaphore->getHandle()};
            constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = waitSemaphores.size();
            submitInfo.pWaitSemaphores = waitSemaphores.data();
            submitInfo.pWaitDstStageMask = waitStages;
            
            std::array commandBuffers = {cmdBuffer->getHandle()};
            submitInfo.commandBufferCount = commandBuffers.size();
            submitInfo.pCommandBuffers = commandBuffers.data();

            std::array signalSemaphores = {renderFinishedSemaphore->getHandle()};
            submitInfo.signalSemaphoreCount = signalSemaphores.size();
            submitInfo.pSignalSemaphores = signalSemaphores.data();
            
            if (graphicsQueue->submit(1, submitInfo, inFlightFence) != VK_SUCCESS)
            {
                NARC_ERROR_RUNTIME("Failed to submit draw command buffer!");
            }

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = signalSemaphores.size();
            presentInfo.pWaitSemaphores = signalSemaphores.data();

            VkSwapchainKHR swapChains[] = {swapchain->getHandle()};
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional

            vkQueuePresentKHR(presentQueue->getHandle(), &presentInfo);
        }
    }

    void recordCommandBuffer(const narc_engine::VulkanFramebuffer* framebuffer, const narc_engine::VulkanSwapChain* swapchain,
                             const narc_engine::VulkanGraphicsPipeline* pipeline)
    {
        cmdBuffer->begin();
        cmdBuffer->beginRenderPass(*framebuffer, *pipeline->getRenderPass());

        cmdBuffer->cmdBindPipeline(*pipeline);

        cmdBuffer->cmdBindVertexBuffers(*vertexBuffer);

        const auto extend = swapchain->getSwapChainExtent();

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extend.width);
        viewport.height = static_cast<float>(extend.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        cmdBuffer->cmdSetViewport(viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = extend;
        cmdBuffer->cmdSetScissor(scissor);

        cmdBuffer->cmdDraw();

        cmdBuffer->endRenderPass();
        cmdBuffer->end();
    }
};

class TestDeviceExtensions final : public narc_engine::IVulkanExtension
{
public:
    TestDeviceExtensions() = default;

    ~TestDeviceExtensions() noexcept override = default;

    void init() override
    {
        //Tests
    }

    void shutdown() override
    {
        //Tests
    }

    NO_DISCARD narc_engine::ExtensionNameList getExtensionNames() const noexcept override
    {
        return {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    }

    NO_DISCARD const void* getCreationInfos() const noexcept override
    {
        return nullptr;
    }

};

class Creator final : public narc_core::ICreator<narc_engine::VulkanSwapChain>
{
public:
    std::function<std::unique_ptr<narc_engine::VulkanSwapChain>()> CreateVulkanSwapChain;

    [[nodiscard]] std::unique_ptr<narc_engine::VulkanSwapChain> create() const noexcept override
    {
        return CreateVulkanSwapChain();
    }

};

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    const auto injector = di::make_injector(
            di::bind<narc_engine::IVulkanSurface>.to<narc_engine::GlfwVulkanSurface>(),
            di::bind<narc_engine::IInstanceService>.to<narc_engine::InstanceService>(),
            di::bind<narc_engine::ISwapchainService>.to<narc_engine::SwapChainService>(),
            di::bind<narc_engine::IDeviceService>.to<narc_engine::DeviceService>(),
            di::bind<narc_engine::IDeviceQueueService>.to<narc_engine::DeviceQueueService>(),
            di::bind<narc_engine::ISurfacesHandler>.to<SurfaceManager>(),
            di::bind<narc_engine::IVulkanMemoryAllocationService>.to<narc_engine::MemoryAllocationService>(),
            di::bind<narc_engine::VulkanSurfacesManager>.to<SurfaceManager>(),
            di::bind<narc_core::ICreator<narc_engine::VulkanSwapChain> >.to<Creator>().in(di::singleton)
            );

    {
        const auto creator = injector.create<std::shared_ptr<Creator> >();
        creator->CreateVulkanSwapChain = [&injector] {
            return injector.create<std::unique_ptr<narc_engine::VulkanSwapChain> >();
        };
    }

    try
    {
        std::vector<std::shared_ptr<narc_engine::IVulkanExtension> > vulkanExtensions;
        vulkanExtensions.push_back(std::make_shared<TestDeviceExtensions>());

        const auto instance = injector.create<std::shared_ptr<narc_engine::VulkanInstance> >();
        instance->setApplicationInfo(narc_engine::ApplicationInfo{
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });

        const auto device = injector.create<std::shared_ptr<narc_engine::VulkanDevice> >();
        device->setPhysicalDeviceCriteria(narc_engine::PhysicalDeviceCriteria{
                .RequireGeometryShader = false,
                .RequireDiscreteGPU = false,
                .DeviceRequiredExtensions = vulkanExtensions,
                .PreferDiscreteGPU = true
        });

        const auto surfacesManager = injector.create<std::shared_ptr<narc_engine::VulkanSurfacesManager> >();
        surfacesManager->setDevice(device);

        const auto cmdPool = injector.create<std::shared_ptr<narc_engine::VulkanCommandPool> >();
        auto mainWindow = injector.create<std::unique_ptr<narc_engine::IVulkanSurface> >();

        auto imageAvailableSemaphore = injector.create<narc_engine::VulkanSemaphore>();
        auto renderFinishedSemaphore = injector.create<narc_engine::VulkanSemaphore>();
        auto inFlightFence = injector.create<narc_engine::VulkanFence>();

        instance->setApplicationInfo(narc_engine::ApplicationInfo{
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });

        instance->init();
        mainWindow->init();
        surfacesManager->pushSurface(mainWindow);
        device->init();
        surfacesManager->init();
        const auto surf = dynamic_cast<SurfaceManager*>(surfacesManager.get());
        surf->imageAvailableSemaphore = &imageAvailableSemaphore;
        surf->inFlightFence = &inFlightFence;
        surf->renderFinishedSemaphore = &renderFinishedSemaphore;
        surf->graphicsQueue = const_cast<narc_engine::VulkanQueue*>(device->getGraphicsQueue());
        surf->presentQueue = const_cast<narc_engine::VulkanQueue*>(device->getPresentQueue());
        surf->setDevice(device);

        cmdPool->init();

        imageAvailableSemaphore.init();
        renderFinishedSemaphore.init();
        inFlightFence.init();

        {
            const auto cmdBuffer = cmdPool->allocateCommandBuffer();
            surf->cmdBuffer = cmdBuffer.get();

            auto vertexBuffer = injector.create<std::unique_ptr<narc_engine::VulkanVertexBuffer> >();
            auto stagingBuffer = injector.create<std::unique_ptr<narc_engine::VulkanStagingBuffer> >();
            stagingBuffer->allocate(narc_engine::s_vertices.size() * sizeof(narc_engine::s_vertices[0]));
            stagingBuffer->setData(narc_engine::s_vertices.data(), narc_engine::s_vertices.size() * sizeof(narc_engine::s_vertices[0]));
            stagingBuffer->copyTo(*cmdPool, *device->getGraphicsQueue(), *vertexBuffer);
            stagingBuffer->deallocate();
            
            //Store size in class
            surf->vertexBuffer = vertexBuffer.get();

            while (!surfacesManager->getMainSurface()->shouldClose())
            {
                glfwPollEvents();
                
                surfacesManager->updateSurfaces();
            }
            device->waitIdle();
        }

        inFlightFence.shutdown();
        renderFinishedSemaphore.shutdown();
        imageAvailableSemaphore.shutdown();

        cmdPool->shutdown();

        surfacesManager->shutdown();
        device->shutdown();
        instance->shutdown();
    }

    catch
    (
        const std::exception& e
    )
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
