#include "platform/vulkan/ContextVulkan.h"

#include "platform/vulkan/VulkanUtils.h"

#include "platform/vulkan/context/ExtensionVulkan.h"

namespace narc_engine
{
#define MINIMUM_VK_VERSION VK_API_VERSION_1_1

    //----- VALIDATION -----
#define VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

    //----- MONITORING -----
#define API_DUMP_LAYER_NAME "VK_LAYER_LUNARG_api_dump"

    ContextVulkan::ContextVulkan()
    {
        m_requiredExtensions.reserve(10);

        m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        m_appInfo.pApplicationName = "No Name Application";
        m_appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        m_appInfo.pEngineName = "NarcEngine";
        m_appInfo.engineVersion = VK_MAKE_VERSION(0, 2, 0);
        m_appInfo.apiVersion = VK_API_VERSION_1_1;

        m_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_createInfo.pApplicationInfo = &m_appInfo;

        addExtensions(CoreExtension, 1);
    }

    ContextVulkan::~ContextVulkan()
    {
        for (auto& ptr : m_requiredExtensions)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    void ContextVulkan::init()
    {
        const uint32_t extensionCount = static_cast<uint32_t>(m_requiredExtensions.size());
        std::vector<const char*> extensions;
        extensions.reserve(m_requiredExtensions.size());
        ExtensionVulkan* lastExtension = nullptr;
        for (uint32_t i = 0; i < extensionCount; ++i)
        {
            ExtensionVulkan* ext = m_requiredExtensions[i];
            extensions.push_back(ext->getName());
            std::cout << "Enabling extension: " << ext->getName() << std::endl;

            const void* createInfo = ext->getCreateInfo();
            if (createInfo == nullptr)
                continue;

            if (lastExtension != nullptr)
            {
                lastExtension->linkNextCreateInfo(createInfo);
            }
            else
            {
                std::cout << "Linking extension: " << ext->getName() << std::endl;
                m_createInfo.pNext = createInfo;
            }

            lastExtension = ext;
        }

        m_createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        m_createInfo.ppEnabledExtensionNames = extensions.data();

        m_createInfo.enabledLayerCount = static_cast<uint32_t>(m_requiredLayers.size());
        m_createInfo.ppEnabledLayerNames = m_requiredLayers.data();

        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create instance!");
        }

        for (const auto& ext : m_requiredExtensions)
        {
            ext->init();
        }
    }

    void ContextVulkan::shutdown()
    {
        for (const auto& ext : m_requiredExtensions)
        {
            ext->shutdown();
        }

        vkDestroyInstance(m_instance, nullptr);
    }

    void ContextVulkan::setApplicationVersion(const uint16_t major, const uint16_t minor, const uint16_t patch)
    {
        m_appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);
    }

    void ContextVulkan::setApplicationName(const char* name) { m_appInfo.pApplicationName = name; }

    RhiResult ContextVulkan::enableExtension(const RhiExtension& extension)
    {
        switch (extension)
        {
        case RhiExtension::Core:
            return RHI_SUCCESS;

        case RhiExtension::DebugUtils:
            m_requiredExtensions.push_back(new DebugExtensionVulkan(this));
            return RHI_SUCCESS;

        case RhiExtension::Surface:
            m_requiredExtensions.push_back(new SurfaceExtensionVulkan(this));
            return RHI_SUCCESS;

        case RhiExtension::ExtendedDevicesProperties:
            m_requiredExtensions.push_back(new ExtendedDevicesPropertiesExtensionVulkan(this));
            return RHI_SUCCESS;

        case RhiExtension::ExtendedSurfaceCapabilities:
            m_requiredExtensions.push_back(new SurfaceExtensionVulkan(this));
            m_requiredExtensions.push_back(new ExtendedSurfaceCapabilitiesExtensionVulkan(this));
            return RHI_SUCCESS;

        default:
            return RHI_FAILURE;
        }
    }

    RhiResult ContextVulkan::enableLayer(const RhiLayer& extension)
    {
        switch (extension)
        {
        case RhiLayer::Validation:
            m_requiredLayers.push_back(VALIDATION_LAYER_NAME);
            return RHI_SUCCESS;

        case RhiLayer::Monitoring:
            m_requiredLayers.push_back(API_DUMP_LAYER_NAME);
            return RHI_SUCCESS;

        default:
            return RHI_FAILURE;
        }
    }
} // namespace narc_engine
