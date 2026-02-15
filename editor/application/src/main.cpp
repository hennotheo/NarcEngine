#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

#include "Ubo.h"
#include "SurfaceManager.h"
#include "TestDeviceExtension.h"
#include "layers/VulkanValidationLogger.h"

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
            di::bind<narc_engine::ISurface>.to<narc_engine::GlfwVulkanSurface>(),
            di::bind<narc_engine::IInstanceService>.to<narc_engine::InstanceService>(),
            di::bind<narc_engine::ISwapchainService>.to<narc_engine::SwapChainService>(),
            di::bind<narc_engine::IDeviceService>.to<narc_engine::DeviceService>(),
            di::bind<narc_engine::IDeviceQueueService>.to<narc_engine::DeviceQueueService>(),
            di::bind<narc_engine::ISurfacesHandler>.to<SurfaceManager>(),
            di::bind<narc_engine::IVulkanMemoryAllocationService>.to<narc_engine::MemoryAllocationService>(),
            di::bind<narc_engine::ICmdService>.to<narc_engine::CmdService>(),
            di::bind<narc_engine::VulkanSurfacesManager>.to<SurfaceManager>(),
            di::bind<narc_core::ICreator<narc_engine::VulkanSwapChain>>.to<Creator>().in(di::singleton)
            );

    {
        const auto creator = injector.create<std::shared_ptr<Creator>>();
        creator->CreateVulkanSwapChain = [&injector] {
            return injector.create<std::unique_ptr<narc_engine::VulkanSwapChain>>();
        };
    }

    try
    {
        std::vector<std::shared_ptr<narc_engine::IVulkanExtension>> vulkanExtensions;
        vulkanExtensions.push_back(std::make_shared<TestDeviceExtensions>());

        const auto instance = injector.create<std::shared_ptr<narc_engine::VulkanInstance>>();
        instance->setApplicationInfo(narc_engine::ApplicationInfo{
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });
        instance->addExtension(injector.create<std::unique_ptr<narc_engine::VulkanGlfwExtension>>());
        instance->addExtension(injector.create<std::unique_ptr<narc_engine::VulkanValidationLogger>>());

        const auto device = injector.create<std::shared_ptr<narc_engine::VulkanDevice>>();
        device->setPhysicalDeviceCriteria(narc_engine::PhysicalDeviceCriteria{
                .RequireGeometryShader = false,
                .RequireDiscreteGPU = false,
                .DeviceRequiredExtensions = vulkanExtensions,
                .PreferDiscreteGPU = true
        });

        const auto surfacesManager = injector.create<std::shared_ptr<narc_engine::VulkanSurfacesManager>>();
        surfacesManager->setDevice(device);

        const auto cmdPool = injector.create<std::shared_ptr<narc_engine::VulkanCommandPool>>();
        auto mainWindow = injector.create<std::unique_ptr<narc_engine::ISurface>>();

        std::vector<std::unique_ptr<narc_engine::VulkanSemaphore>> imageAvailableSemaphores;
        std::vector<std::unique_ptr<narc_engine::VulkanSemaphore>> renderFinishedSemaphores;
        std::vector<std::unique_ptr<narc_engine::VulkanFence>> inFlightFences;
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            auto imageAvailableSemaphore = injector.create<std::unique_ptr<narc_engine::VulkanSemaphore>>();
            auto renderFinishedSemaphore = injector.create<std::unique_ptr<narc_engine::VulkanSemaphore>>();
            auto inFlightFence = injector.create<std::unique_ptr<narc_engine::VulkanFence>>();

            imageAvailableSemaphores.push_back(std::move(imageAvailableSemaphore));
            renderFinishedSemaphores.push_back(std::move(renderFinishedSemaphore));
            inFlightFences.push_back(std::move(inFlightFence));
        }

        instance->setApplicationInfo(narc_engine::ApplicationInfo{
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });

        auto descriptorSetPool = injector.create<std::shared_ptr<narc_engine::VulkanDescriptorPool>>();
        descriptorSetPool->setDescriptorCount(MAX_FRAMES_IN_FLIGHT);

        auto descriptorSetLayout = injector.create<narc_engine::VulkanDescriptorSetLayout>();
        descriptorSetLayout.addBinding({
                .BindingIndex = 0,
                .Stage = narc_engine::Vertex,
                .Type = narc_engine::UniformBuffer
        });
        descriptorSetLayout.addBinding({
                .BindingIndex = 1,
                .Stage = narc_engine::Fragment,
                .Type = narc_engine::Sampler
        });

        instance->init();
        mainWindow->init();

        const auto surfaceComponent = surfacesManager->pushSurface(mainWindow);
        surfaceComponent.Layout->addDescriptorSetLayoutBinding(&descriptorSetLayout);
        device->init();
        descriptorSetLayout.init();
        surfacesManager->init();
        const auto surf = dynamic_cast<SurfaceManager*>(surfacesManager.get());
        surf->descriptor_set_layout = &descriptorSetLayout;
        surf->imageAvailableSemaphores = &imageAvailableSemaphores;
        surf->inFlightFences = &inFlightFences;
        surf->renderFinishedSemaphores = &renderFinishedSemaphores;
        surf->graphicsQueue = const_cast<narc_engine::VulkanQueue*>(device->getGraphicsQueue());
        surf->presentQueue = const_cast<narc_engine::VulkanQueue*>(device->getPresentQueue());
        surf->setDevice(device);

        cmdPool->init();

        {
            auto vertexBuffer = injector.create<std::unique_ptr<narc_engine::VulkanVertexBuffer>>();
            auto stagingBuffer = injector.create<std::unique_ptr<narc_engine::VulkanStagingBuffer>>();
            stagingBuffer->allocate(narc_engine::s_vertices.size() * sizeof(narc_engine::s_vertices[0]));
            stagingBuffer->setData(narc_engine::s_vertices.data());
            stagingBuffer->copyToBuffer(*vertexBuffer);
            stagingBuffer->deallocate();

            auto indexBuffer = injector.create<std::unique_ptr<narc_engine::VulkanIndexBuffer>>();
            stagingBuffer->allocate(narc_engine::s_indices.size() * sizeof(narc_engine::s_indices[0]));
            stagingBuffer->setData(narc_engine::s_indices.data());
            stagingBuffer->copyToBuffer(*indexBuffer);
            stagingBuffer->deallocate();

            auto textureImage = injector.create<std::unique_ptr<narc_engine::VulkanTextureImage>>();
            textureImage->setpath(std::string("textures/tex_test_uv_0.png"));
            textureImage->init();

            //Store size in class
            surf->vertexBuffer = vertexBuffer.get();
            surf->indexBuffer = indexBuffer.get();


            std::vector<narc_engine::VulkanUniformBuffer> uniformBuffers;
            uniformBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                uniformBuffers.push_back(injector.create<narc_engine::VulkanUniformBuffer>());
            }

            for (auto& uniform_buffer: uniformBuffers)
            {
                uniform_buffer.allocate(sizeof(UniformBufferObject));
            }

            descriptorSetPool->init();

            auto sets = descriptorSetPool->allocateDescriptorSet(std::vector(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout));
            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformBuffers[i].getHandle();
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(UniformBufferObject);

                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = textureImage->getView();
                imageInfo.sampler = textureImage->getSampler();

                std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = sets[i].getHandle();
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;
                descriptorWrites[0].pImageInfo = nullptr;
                descriptorWrites[0].pTexelBufferView = nullptr;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = sets[i].getHandle();
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pImageInfo = &imageInfo;

                vkUpdateDescriptorSets(device->getHandle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
            surf->descriptor_sets = sets;

            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                imageAvailableSemaphores[i]->init();
                renderFinishedSemaphores[i]->init();
                inFlightFences[i]->init();
            }


            std::vector<std::unique_ptr<narc_engine::VulkanCommandBuffer>> commandBuffers;
            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                commandBuffers.push_back(cmdPool->allocateCommandBuffer());
            }
            surf->cmdBuffer = &commandBuffers;

            surf->uniform_buffers = &uniformBuffers;


            //---------------- RUNTIME ----------------------

            while (!surfacesManager->getMainSurface()->shouldClose())
            {
                glfwPollEvents();

                surfacesManager->updateSurfaces();
            }
            device->waitIdle();

            //---------------- END RUNTIME ----------------------

            textureImage->shutdown();

            for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                imageAvailableSemaphores[i]->shutdown();
                renderFinishedSemaphores[i]->shutdown();
                inFlightFences[i]->shutdown();
            }

            descriptorSetPool->shutdown();

            for (auto& uniform_buffer: uniformBuffers)
            {
                uniform_buffer.deallocate();
            }
        }


        cmdPool->shutdown();

        surfacesManager->shutdown();
        descriptorSetLayout.shutdown();
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
