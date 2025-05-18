#include "renderer/render_graph/DrawCall.h"

#include "platform/vulkan/CommandBuffer.h"
#include "models/Material.h"
#include "models/PushConstants.h"
#include "renderer/GraphicsPipeline.h"
#include "renderer/render_graph/contexts/RenderContext.h"
#include "models/Mesh.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine
{
    DrawCall::DrawCall(const Material* material, const GraphicsPipeline* pipeline)
        : m_material(material), m_pipeline(pipeline), m_meshes()
    {
    }

    void DrawCall::record(const CommandBuffer* cmd, const RenderContext* ctx)
    {
        cmd->cmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_pipeline->getLayout(), 0, 1,
            &ctx->FrameHandler->getDescriptorSet(m_material->getId()), 0, nullptr);

        uint32_t drawCallIndex = 0;
        for (const Mesh* mesh : m_meshes)
        {
            PushConstants pushConstants{};
            pushConstants.objectIndex = drawCallIndex;

            VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
            VkDeviceSize offsets[] = { 0 };
            cmd->cmdBindVertexBuffers(0, 1, vertexBuffers, offsets);
            cmd->cmdBindIndexBuffer(mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

            cmd->cmdPushConstants(m_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

            cmd->cmdDrawIndexed(mesh->getIndexCount(), 1, 0, 0, 0);
            drawCallIndex++;
        }
    }
} // namespace narc_engine