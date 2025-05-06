#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine
{
    class RenderContext;
    class CommandBuffer;
    class GraphicsPipeline;
    class Material;
    class Mesh;

    class DrawCall
    {
    public:
        DrawCall(const Material* material, const GraphicsPipeline* pipeline);
        ~DrawCall() = default;

        void setMeshes(const std::vector<const Mesh*> meshes) { m_meshes = meshes; }
        void record(const CommandBuffer* cmd, const RenderContext* ctx);

    private:
        const Material* m_material;
        const GraphicsPipeline* m_pipeline;
        const std::vector<const Mesh*> m_meshes;
    };
} // namespace narc_engine