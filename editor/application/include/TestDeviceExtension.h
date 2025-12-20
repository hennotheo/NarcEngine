//
// Created by theo on 12/20/25.
//

#pragma once

class TestDeviceExtensions final : public narc_engine::IVulkanExtension
{
public:
    TestDeviceExtensions() = default;

    ~TestDeviceExtensions() noexcept override = default;

    void init() override
    {
        //Tests
    }

    void shutdown() override
    {
        //Tests
    }

    NO_DISCARD narc_engine::ExtensionNameList getExtensionNames() const noexcept override
    {
        return {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    }

    NO_DISCARD const void* getCreationInfos() const noexcept override
    {
        return nullptr;
    }

};