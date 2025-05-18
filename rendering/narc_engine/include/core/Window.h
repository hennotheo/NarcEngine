#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "render_graph/RenderGraph.h"
#include "renderer/MultiFrameManager.h"
#include "EngineBinder.h"
#include "platform/vulkan/SwapChain.h"

#include "interfaces/ISurfaceProvider.h"
#include "renderer/SwapChainSupportDetails.h"

namespace narc_engine
{
    class EngineInstance;
    class IEngineCallbacks;
    class EngineBuilder;
    class PhysicalDevice;
    class LogicalDevice;

    class Window : public ISurfaceProvider
    {
        friend class EngineBinder;

    public:
        explicit Window(const EngineInstance* engineInstance);
        ~Window();

        NARC_IMPL_IGETTER(VkSurfaceKHR, m_surface)
        GETTER const void* getSurfaceHandler() const override { return m_window; }
        GETTER bool shouldClose() const { return m_shouldClose; }

        void getValidFramebufferSize(int* width, int* height) const override;
        void getFramebufferSize(int* width, int* height) const override;

        void initRenderingSystem(const EngineBuilder* builder);
        void pollEvents();
        void render();

        void addRenderer(const Renderer* renderer);

        static const char** getRequiredInstanceExtensions(uint32_t* glfwExtensionCount);

    private:
        GLFWwindow* m_window;
        VkSurfaceKHR m_surface;

        std::unique_ptr<SwapChain> m_swapchain;
        std::unique_ptr<MultiFrameManager> m_frameManager;
        std::unique_ptr<RenderGraph> m_renderGraph;

        narc_core::Event<int, int, int, int> m_onKeyboardEvent;
        narc_core::Event<int, int, int> m_onMouseEvent;

        // TODO : REMOVE THIS
        double m_mouseXpos = 0.0;
        double m_mouseYpos = 0.0;
        double m_time = 0.0;
        bool m_shouldClose = false;
        bool m_framebufferResized = false;

        const EngineInstance* m_engineInstance;
        const PhysicalDevice* m_physicalDevice;
        const LogicalDevice* m_logicalDevice;

    private:
        void present(const SignalSemaphores& signalSemaphores, uint32_t currentImageIndex);

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        static void onKeyboardInputPerformed(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void onMouseInputPerformed(GLFWwindow* window, int button, int action, int mods);
    };
}