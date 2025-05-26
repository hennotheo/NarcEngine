//
// Created by theoh on 23/05/2025.
//

#include "rhi/tests/test_rhi.h"
#include "test_context_base.h"

#include "backend_vulkan/ContextVulkan.h"

class RhiContextTestVulkan : public RhiTest
{
protected:
    RendererApiType getTestedApi() override { return RendererApiType::Vulkan; }
};

/**
 * @file test_context_vulkan.cpp
 * @brief Unit tests for Vulkan-specific ContextRhi functionality.
 */

TEST_F(RhiContextTestVulkan, ContextRhi_Creation)
{
    /**
     * @test Verifies that a ContextRhi object can be successfully created for the Vulkan API.
     * @details Ensures that the created context is not null.
     */
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    ASSERT_NE(context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
}

TEST_F(RhiContextTestVulkan, ContextRhi_InitShutdown)
{
    /**
     * @test Verifies that the ContextRhi object can be initialized and shut down without exceptions.
     * @details Ensures that the init() and shutdown() methods do not throw any exceptions.
     */
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    EXPECT_NO_THROW(context->init()) << "ContextRhi initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "ContextRhi shutdown threw an exception";
}

TEST_F(RhiContextTestVulkan, ContextRhi_InitInstanceExists)
{
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    const auto vkContext = context->getContextVulkan();
    context->init();
    EXPECT_TRUE(vkContext->getVkInstance() != nullptr);
    context->shutdown();
}

TEST_F(RhiContextTestVulkan, ContextRhi_Core_Enabled)
{
    /**
     * @test Checks if the Core extension is enabled for the Vulkan ContextRhi.
     * @details Ensures that the isExtensionEnabled() method returns true for the Core extension.
     */
    const ContextRhiPtr context = createContextRhi(getTestedApi());
    EXPECT_TRUE(context->isExtensionEnabled(RhiExtension::Core));
}

TEST_P(RhiExtensionsTest, Extensions_Enabled)
{
    /**
     * @test Verifies that a specific extension can be enabled for the Vulkan ContextRhi.
     * @param extension The extension to be tested, provided by the test parameterization.
     * @details Ensures that the isExtensionEnabled() method returns true after adding the extension.
     */
    const ExtensionParam params = GetParam();
    const ContextRhiPtr context = createContextRhi(RendererApiType::Vulkan);

    context->addExtensions(params.extensions.data(), params.extensions.size());

    for (auto& extension : params.extensions)
    {
        EXPECT_TRUE(context->isExtensionEnabled(extension)) << "Extension " << static_cast<int>(extension) << " should be enabled";
    }
}

TEST_P(RhiExtensionsTest, Extensions_InitShutdown)
{
    /**
     * @test Verifies that a ContextRhi with a specific extension can be initialized and shut down without exceptions.
     * @param extension The extension to be tested, provided by the test parameterization.
     * @details Ensures that the init() and shutdown() methods do not throw any exceptions after adding the extension.
     */
    const ExtensionParam params = GetParam();
    const ContextRhiPtr context = createContextRhi(RendererApiType::Vulkan);

    constexpr RhiExtension debug = RhiExtension::DebugUtils;
    constexpr RhiLayer validation = RhiLayer::Validation;
    context->addExtensions(&debug, 1);
    context->addLayers(&validation, 1);

    context->addExtensions(params.extensions.data(), params.extensions.size());

    EXPECT_NO_THROW(context->init()) << "ContextRhi initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "ContextRhi shutdown threw an exception";
}

INSTANTIATE_TEST_SUITE_P(
    RhiExtensionsVulkanTest,
    RhiExtensionsTest,
    /**
     * @brief Parameterizes the RhiExtensionsTest with a set of Vulkan-specific extensions.
     * @details Tests are run for each extension in the provided list.
     */
    ::testing::Values(
        ExtensionParam {"Core", {RhiExtension::Core}},
        ExtensionParam {"Debug", {RhiExtension::DebugUtils}},
        ExtensionParam {"ExtendedDevicesProperties", {RhiExtension::ExtendedDevicesProperties}},
        ExtensionParam {"ExtendedSurfaceCapabilities", {RhiExtension::ExtendedSurfaceCapabilities}}
    ),
    [](const ::testing::TestParamInfo<ExtensionParam>& info) {
    return info.param.name;
    }
);

/**
 * @brief Tests if a specific layer can be enabled for the Vulkan ContextRhi.
 * @details Ensures that the isLayerEnabled() method returns true after adding the layer.
 * @test Parameterized test for Vulkan layers.
 */
TEST_P(RhiLayersTest, Layer_Enabled)
{
    /**
     * @test Verifies that a specific layer can be enabled for the Vulkan ContextRhi.
     * @param layer The layer to be tested, provided by the test parameterization.
     * @details Ensures that the isLayerEnabled() method returns true after adding the layer.
     */
    const RhiLayer layer = GetParam();
    const ContextRhiPtr context = createContextRhi(RendererApiType::Vulkan);

    context->addLayers(&layer, 1);

    EXPECT_TRUE(context->isLayerEnabled(layer));
}

/**
 * @brief Tests if a ContextRhi with a specific layer can be initialized and shut down without exceptions.
 * @details Ensures that the init() and shutdown() methods do not throw any exceptions after adding the layer.
 * @test Parameterized test for Vulkan layers.
 */
TEST_P(RhiLayersTest, Layer_InitShutdown)
{
    /**
     * @test Verifies that a ContextRhi with a specific layer can be initialized and shut down without exceptions.
     * @param layer The layer to be tested, provided by the test parameterization.
     * @details Ensures that the init() and shutdown() methods do not throw any exceptions after adding the layer.
     */
    const RhiLayer layer = GetParam();
    const ContextRhiPtr context = createContextRhi(RendererApiType::Vulkan);

    const std::vector<RhiExtension> extension = {RhiExtension::Core, RhiExtension::DebugUtils};
    context->addExtensions(extension.data(), extension.size());
    context->addLayers(&layer, 1);

    EXPECT_NO_THROW(context->init()) << "ContextRhi initialization threw an exception";
    EXPECT_NO_THROW(context->shutdown()) << "ContextRhi shutdown threw an exception";
}

/**
 * @brief Parameterizes the RhiLayersTest with a set of Vulkan-specific layers.
 * @details Tests are run for each layer in the provided list.
 */
INSTANTIATE_TEST_SUITE_P(
    RhiLayersVulkanTest,
    RhiLayersTest,
    ::testing::Values(RhiLayer::Validation, RhiLayer::Monitoring)
    );