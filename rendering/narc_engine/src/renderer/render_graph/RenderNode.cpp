#include "renderer/render_graph/RenderNode.h"

#include <vulkan/vulkan.h>

#include "CommandBuffer.h"
#include "models/Material.h"
#include "renderer/render_graph/RenderContext.h"
#include "models/PushConstants.h"
#include "models/Mesh.h"
#include "buffers/GraphicsBuffer.h"

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
            const Renderer* renderer = ctx->Renderers[i];
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

        for (const auto& [mat, renderers] : uniqueMaterials)
        {
            cmd->cmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 0, 1,
                &ctx->DescriptorSets->data()[mat->getMaterialID()], 0, nullptr);

            uint32_t drawCallIndex = 0;
            for (const auto& renderer : renderers)
            {                    
                PushConstants pushConstants{};
                pushConstants.objectIndex = drawCallIndex;

                const Mesh* mesh = renderer->getMesh();
                VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
                VkDeviceSize offsets[] = { 0 };
                cmd->cmdBindVertexBuffers(0, 1, vertexBuffers, offsets);
                cmd->cmdBindIndexBuffer(mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

                cmd->cmdPushConstants(m_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

                cmd->cmdDrawIndexed(mesh->getIndexCount(), 1, 0, 0, 0);
                drawCallIndex++;
            }
        }
    }
} // namespace narc_engine