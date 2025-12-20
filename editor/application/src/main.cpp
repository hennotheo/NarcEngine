#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

#include "Ubo.h"
#include "SurfaceManager.h"
#include "TestDeviceExtension.h"

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

        std::vector<std::unique_ptr<narc_engine::VulkanSemaphore> > imageAvailableSemaphores;
        std::vector<std::unique_ptr<narc_engine::VulkanSemaphore> > renderFinishedSemaphores;
        std::vector<std::unique_ptr<narc_engine::VulkanFence> > inFlightFences;
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            auto imageAvailableSemaphore = injector.create<std::unique_ptr<narc_engine::VulkanSemaphore> >();
            auto renderFinishedSemaphore = injector.create<std::unique_ptr<narc_engine::VulkanSemaphore> >();
            auto inFlightFence = injector.create<std::unique_ptr<narc_engine::VulkanFence> >();

            imageAvailableSemaphores.push_back(std::move(imageAvailableSemaphore));
            renderFinishedSemaphores.push_back(std::move(renderFinishedSemaphore));
            inFlightFences.push_back(std::move(inFlightFence));
        }

        instance->setApplicationInfo(narc_engine::ApplicationInfo{
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });

        auto descriptorSetPool = injector.create<std::shared_ptr<narc_engine::VulkanDescriptorPool> >();
        descriptorSetPool->setDescriptorCount(MAX_FRAMES_IN_FLIGHT);
        auto descriptorSetLayout = injector.create<narc_engine::VulkanDescriptorSetLayout>();

        instance->init();
        mainWindow->init();
        surfacesManager->pushSurface(mainWindow);
        device->init();
        surfacesManager->init();
        descriptorSetPool->init();
        descriptorSetLayout.init();
        const auto surf = dynamic_cast<SurfaceManager*>(surfacesManager.get());
        surf->descriptor_set_layout = &descriptorSetLayout;
        surf->imageAvailableSemaphores = &imageAvailableSemaphores;
        surf->inFlightFences = &inFlightFences;
        surf->renderFinishedSemaphores = &renderFinishedSemaphores;
        surf->graphicsQueue = const_cast<narc_engine::VulkanQueue*>(device->getGraphicsQueue());
        surf->presentQueue = const_cast<narc_engine::VulkanQueue*>(device->getPresentQueue());
        surf->setDevice(device);

        cmdPool->init();

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            imageAvailableSemaphores[i]->init();
            renderFinishedSemaphores[i]->init();
            inFlightFences[i]->init();
        }

        std::vector<narc_engine::VulkanUniformBuffer> uniformBuffers;
        uniformBuffers.reserve(10);
        for (int i = 0; i < 10; ++i)
        {
            uniformBuffers.push_back(injector.create<narc_engine::VulkanUniformBuffer>());
        }

        {
            std::vector<std::unique_ptr<narc_engine::VulkanCommandBuffer> > commandBuffers;
            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                commandBuffers.push_back(cmdPool->allocateCommandBuffer());
            }
            surf->cmdBuffer = &commandBuffers;

            auto vertexBuffer = injector.create<std::unique_ptr<narc_engine::VulkanVertexBuffer> >();
            auto stagingBuffer = injector.create<std::unique_ptr<narc_engine::VulkanStagingBuffer> >();
            stagingBuffer->allocate(narc_engine::s_vertices.size() * sizeof(narc_engine::s_vertices[0]));
            stagingBuffer->setData(narc_engine::s_vertices.data(), narc_engine::s_vertices.size() * sizeof(narc_engine::s_vertices[0]));
            stagingBuffer->copyTo(*cmdPool, *device->getGraphicsQueue(), *vertexBuffer);
            stagingBuffer->deallocate();

            auto indexBuffer = injector.create<std::unique_ptr<narc_engine::VulkanIndexBuffer> >();
            stagingBuffer->allocate(narc_engine::s_indices.size() * sizeof(narc_engine::s_indices[0]));
            stagingBuffer->setData(narc_engine::s_indices.data(), narc_engine::s_indices.size() * sizeof(narc_engine::s_indices[0]));
            stagingBuffer->copyTo(*cmdPool, *device->getGraphicsQueue(), *indexBuffer);
            stagingBuffer->deallocate();

            //Store size in class
            surf->vertexBuffer = vertexBuffer.get();
            surf->indexBuffer = indexBuffer.get();

            for (auto& uniform_buffer: uniformBuffers)
            {
                uniform_buffer.allocate(sizeof(UniformBufferObject));
            }

            surf->uniform_buffers = &uniformBuffers;

            while (!surfacesManager->getMainSurface()->shouldClose())
            {
                glfwPollEvents();

                surfacesManager->updateSurfaces();
            }
            device->waitIdle();

            for (auto& uniform_buffer: uniformBuffers)
            {
                uniform_buffer.deallocate();
            }
        }

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            imageAvailableSemaphores[i]->shutdown();
            renderFinishedSemaphores[i]->shutdown();
            inFlightFences[i]->shutdown();
        }

        cmdPool->shutdown();

        descriptorSetLayout.shutdown();
        descriptorSetPool->shutdown();
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
