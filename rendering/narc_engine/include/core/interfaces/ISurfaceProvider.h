#pragma once 

#include <vulkan/vulkan_core.h>

#include "ISurfaceObserver.h"

namespace narc_engine {
    class ISurfaceProvider : public narc_core::IGetter<VkSurfaceKHR>
    {
        friend class UiRenderer;

    public:
        virtual ~ISurfaceProvider() = default;

        GETTER virtual const void* getSurfaceHandler() const = 0;
        virtual void getValidFramebufferSize(int* width, int* height) const = 0;
        virtual void getFramebufferSize(int* width, int* height) const = 0;

        virtual void pollEvents() = 0;

        inline void attach(ISurfaceObserver* listener)
        {
            m_listeners.push_back(listener);
        }

    protected:
        inline void notifyFramebufferResized(int width, int height)
        {
            for (auto& listener : m_listeners)
            {
                listener->onSurfaceFramebufferResized(width, height);
            }
        }

    private:
        std::vector<ISurfaceObserver*> m_listeners;
    };
} // namespace narc_engine