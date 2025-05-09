#include "renderer/render_graph/RenderNode.h"

#include <vulkan/vulkan.h>

#include "CommandBuffer.h"
#include "models/Material.h"
#include "renderer/render_graph/RenderContext.h"
#include "models/Mesh.h"
#include "buffers/GraphicsBuffer.h"
#include "renderer/render_graph/DrawCall.h"

#include <NarcLog.h>

namespace narc_engine
{
    RenderNode::RenderNode(const RenderPass* renderPass, const Shader* vertShader, const Shader* fragShader)
    {
        m_pipeline = std::make_unique<GraphicsPipeline>(renderPass, vertShader, fragShader);
    }

    //TODO change to pure virtual function
    void RenderNode::record(const CommandBuffer* cmd, const RenderContext* ctx)
    {
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)ctx->SwapChainExtent.width;
        viewport.height = (float)ctx->SwapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        cmd->cmdSetViewport(&viewport, 0, 1);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = ctx->SwapChainExtent;
        cmd->cmdSetScissor(&scissor, 0, 1);

        m_pipeline->bindPipeline(cmd);

        //ALL MAT MUST HAVE SAME SHADERS FOR NOW
        std::unordered_map<const Material*, std::vector<const Renderer*>> uniqueMaterials;
        for (size_t i = 0; i < ctx->RenderersCount; i++)
        {
            const Renderer* renderer = ctx->Renderers->at(i);
            const Material* material = renderer->getMaterial();
            if (uniqueMaterials.find(material) == uniqueMaterials.end())
            {
                uniqueMaterials[material] = std::vector<const Renderer*>(1, renderer);
            }
            else
            {
                uniqueMaterials[material].push_back(renderer);
            }
        }

        std::vector<DrawCall> drawCalls;
        for (const auto& [mat, renderers] : uniqueMaterials)
        {
            DrawCall drawCall(mat, m_pipeline.get());
            auto it = renderers
                | std::views::transform([](const Renderer* renderer) { return renderer->getMesh(); });
            std::vector<const Mesh*> meshes(it.begin(), it.end());

            drawCall.setMeshes(meshes);
            drawCalls.push_back(drawCall);
        }

        for (DrawCall& drawCall : drawCalls)
        {
            drawCall.record(cmd, ctx);
        }
    }
} // namespace narc_engine