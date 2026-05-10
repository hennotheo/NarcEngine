//
// Created by theo on 2/18/26.
//

#include "NarcImguiWrapper.h"

namespace narc_engine {
    ImguiWrapper::ImguiWrapper(std::unique_ptr<ImGuiBackend> backend) :
        m_backend(std::move(backend))
    {
    }

    ImguiWrapper::~ImguiWrapper() = default;

    void ImguiWrapper::init()
    {
        m_backend->init();
    }

    void ImguiWrapper::shutdown()
    {
        m_backend->shutdown();
    }

    void ImguiWrapper::newFrame()
    {
        m_backend->newFrame();

        drawDockspace();
    }

    void ImguiWrapper::startWindow(const std::string& name)
    {
        ImGui::Begin(name.c_str());
    }

    void ImguiWrapper::text(const std::string& content)
    {
        ImGui::Text(content.c_str());
    }

    void ImguiWrapper::spacing()
    {
        ImGui::Spacing();
    }

    void ImguiWrapper::endWindow()
    {
        ImGui::End();
    }

    void ImguiWrapper::endFrame()
    {
        m_backend->endFrame();
    }

    void ImguiWrapper::render(const ICommandBuffer* cmdBuffer)
    {
        auto* vulkanBackend = narc_core::backend_cast<ImguiVulkanWrapper, ImGuiBackend>(m_backend.get());
        auto* cmd = narc_core::backend_cast<VulkanCommandBuffer, ICommandBuffer>(cmdBuffer);

        vulkanBackend->render(cmd);
    }

    void ImguiWrapper::drawDockspace()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground;

        ImGui::Begin("DockSpace", nullptr, flags);

        ImGuiID dockspaceId = ImGui::GetID("MainDockSpace");

        ImGui::DockSpace(
            dockspaceId,
            ImVec2(0.0f, 0.0f),
            ImGuiDockNodeFlags_PassthruCentralNode
        );

        ImGui::End();
    }
}
