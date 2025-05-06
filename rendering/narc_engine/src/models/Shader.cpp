//
// Created by theoh on 3/14/2025.
//

#include "models/Shader.h"

#include "Engine.h"

namespace narc_engine {
    Shader::Shader(const std::string& filename) : DeviceComponent()
    {
        const auto code = narc_io::FileReader::readFile(filename);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(getVkDevice(), &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create shader module!");
        }

        createDescriptorSetLayout();
    }

    Shader::~Shader()
    {
        vkDestroyShaderModule(getVkDevice(), m_shaderModule, nullptr);
        vkDestroyDescriptorSetLayout(getVkDevice(), m_descriptorSetLayout, nullptr);
    }

    VkPipelineShaderStageCreateInfo Shader::configureShaderStage(const char* entryPoint, VkShaderStageFlagBits stage) const
    {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = stage;
        vertShaderStageInfo.module = m_shaderModule;
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

        if (vkCreateDescriptorSetLayout(getVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout) !=
            VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create descriptor set layout!");
        }
    }
} // narc_engine
