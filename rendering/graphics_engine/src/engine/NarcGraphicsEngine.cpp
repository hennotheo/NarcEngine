//
// Created by theo on 5/14/26.
//

#include "engine/NarcGraphicsEngine.h"

#include "render_graph/RenderGraph.h"

namespace narc_engine {

    NarcGraphicsEngine::NarcGraphicsEngine(std::unique_ptr<IGraphicsInstance>& graphicsInstance) :
        m_graphicsInstance(std::move(graphicsInstance))
    {
    }

    NarcGraphicsEngine::~NarcGraphicsEngine() = default;

    void NarcGraphicsEngine::init()
    {
        m_graphicsInstance->init();
        m_graphicsInstance->setApplicationInfo({
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });
        m_graphicsInstance->setDeviceCriteria({
                .RequireGeometryShader = false,
                .RequireDiscreteGPU = false,
                .DeviceRequiredExtensions = {},
                .PreferDiscreteGPU = true
        });

        m_renderGraph = std::make_unique<RenderGraph>(m_graphicsInstance.get());

        auto hdr = m_renderGraph->createTexture("Hdr", RenderGraphTextureDesc{
                                             .width = 54,
                                             .height = 54,
                                     });

        const auto imageStream = narc_io::FileReaderService::readImage("textures/tex_test_uv_0.png");
        auto alloc = ImageAllocationInfo();
        alloc.Extent = {imageStream->getWidth(), imageStream->getHeight()};
        m_image = m_graphicsInstance->createImage(alloc);
        m_image->init();
        auto swapchain = m_renderGraph->importTexture("hh", m_image.get());

        m_renderGraph->compile();

        const IImage& hdrImage = m_renderGraph->resolveImage(hdr);
        const IImage& swapchainImage = m_renderGraph->resolveImage(swapchain);
    }

    void NarcGraphicsEngine::shutdown()
    {
        m_image->shutdown();

        m_renderGraph.reset();
        m_graphicsInstance->shutdown();
    }
}
