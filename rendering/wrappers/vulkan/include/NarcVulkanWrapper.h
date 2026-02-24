//
// Created by theo on 2/13/26.
//

#pragma once

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "VulkanWrapperInterfaces.h"

//TODO: For testing editor
// #include "mapping/mappingToVk.h"
// #include "mapping/mappingFromVk.h"

#include "models/SwapChainSupportInfoVulkan.h"

#include "buffers/VulkanIndexBuffer.h"
#include "buffers/VulkanVertexBuffer.h"
#include "buffers/VulkanUniformBuffer.h"
#include "buffers/VulkanStagingBuffer.h"

#include "command/VulkanCommandPool.h"
#include "command/VulkanCommandBuffer.h"

#include "descriptor/VulkanDescriptorPool.h"
#include "descriptor/VulkanDescriptorSet.h"
#include "descriptor/VulkanDescriptorSetLayout.h"

#include "device/VulkanDevice.h"
#include "device/VulkanQueue.h"

#include "instance/VulkanInstance.h"

#include "pipeline/VulkanGraphicsPipeline.h"
#include "pipeline/VulkanPipelineLayout.h"
#include "pipeline/VulkanRenderPass.h"

#include "sync/VulkanFence.h"
#include "sync/VulkanSemaphore.h"

#include "swapchain/VulkanSwapChain.h"

#include "VulkanFramebuffer.h"
#include "VulkanShaderModule.h"
#include "VulkanTextureImage.h"

#include "services/CmdService.h"
#include "services/MemoryAllocationService.h"

#include "surface/IVulkanSurface.h"

#include "instance/VulkanGraphicsInstance.h"
