//
// Created by theoh on 3/22/2025.
//

#pragma once

class ResourcesTest : public ::testing::Test
{
protected:
    static constexpr const char* TEXTURE_PATH = "textures/test.png";
    static constexpr const char* MODEL_PATH = "models/test.obj";
    static constexpr const char* VERTEX_SHADER_PATH = "shaders/shader_vert.spv";

    IEngine* m_engine = nullptr;

    ResourcesTest()
    {
        m_engine = createEngine();
    }

    ~ResourcesTest() override
    {
        delete m_engine;
        m_engine = nullptr;
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(ResourcesTest, Texture2DCreation)
{
    EXPECT_NO_THROW(m_engine->resourceManager()->createTexture2D(TEXTURE_PATH));
}


TEST_F(ResourcesTest, ShaderModuleCreation)
{
    ShaderModule* shader = nullptr;
    EXPECT_NO_THROW(shader = new ShaderModule(VERTEX_SHADER_PATH););
    EXPECT_NE(nullptr, shader);
}

TEST_F(ResourcesTest, MaterialCreation)
{
    const Material* renderMaterial = nullptr;
    EXPECT_NO_THROW(renderMaterial = new Material(TEXTURE_PATH););
    EXPECT_NE(renderMaterial, nullptr);
}

TEST_F(ResourcesTest, RendererCreation)
{
    const Renderer* renderer = nullptr;
    const Material renderMaterial = Material(TEXTURE_PATH);
    const narc_io::Model3D model = narc_io::FileReader::load3DModel(MODEL_PATH);

    EXPECT_NO_THROW(renderer = new Renderer(&model, &renderMaterial););
    EXPECT_NE(renderer, nullptr);
}