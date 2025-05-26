//
// Created by theoh on 3/14/2025.
//

#include "resources/Shader.h"

#include "Engine.h"
#include "macros/Utils.h"

namespace narc_engine {
    Shader::Shader(const char* vertexShaderFile, const char* fragShaderFile)
        :Resource(ResourceType::Shader), m_vertexShaderModule(vertexShaderFile), m_fragShaderModule(fragShaderFile)
    {
    }

    Shader::~Shader() = default;

    void Shader::onLoad()
    {
        m_vertexShaderModule.load();
        m_fragShaderModule.load();
        createDescriptorSetLayout();
    }

    void Shader::onUnload()
    {
        vkDestroyDescriptorSetLayout(NARC_DEVICE_HANDLE, m_descriptorSetLayout, nullptr);
        m_vertexShaderModule.unload();
        m_fragShaderModule.unload();
    }

    VkPipelineShaderStageCreateInfo Shader::configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const
    {
        const ShaderModule* shaderModule = (stage == VK_SHADER_STAGE_VERTEX_BIT) ? &m_vertexShaderModule : &m_fragShaderModule;

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = stage;
        vertShaderStageInfo.module = shaderModule->get();
        vertShaderStageInfo.pName = entryPoint;

        return vertShaderStageInfo;
    }

    void Shader::createDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(NARC_DEVICE_HANDLE, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create descriptor set layout!");
        }
    }
} // narc_engine
