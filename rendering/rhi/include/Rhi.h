#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "RhiCore.h"

#include "models/ApplicationInfo.h"
#include "models/QueueFamilyIndices.h"
#include "models/PhysicalDeviceCriteria.h"

#include "interfaces/Interfaces.h"

#include "vulkan_wrappers/VulkanInstance.h"
#include "vulkan_wrappers/VulkanDevice.h"
#include "vulkan_wrappers/VulkanQueue.h"
#include "vulkan_wrappers/VulkanSwapChain.h"
#include "vulkan_wrappers/VulkanPipelineLayout.h"
#include "vulkan_wrappers/VulkanGraphicsPipeline.h"
#include "vulkan_wrappers/VulkanShaderModule.h"
#include "vulkan_wrappers/VulkanRenderPass.h"
#include "vulkan_wrappers/VulkanFramebuffer.h"
#include "vulkan_wrappers/VulkanCommandPool.h"
#include "vulkan_wrappers/VulkanCommandBuffer.h"
#include "vulkan_wrappers/VulkanSemaphore.h"
#include "vulkan_wrappers/VulkanFence.h"
#include "vulkan_wrappers/buffers/VulkanVertexBuffer.h"
#include "vulkan_wrappers/buffers/VulkanStagingBuffer.h"
#include "vulkan_wrappers/buffers/VulkanIndexBuffer.h"
#include "GlfwVulkanSurface.h"
#include "VulkanSurfacesManager.h"

//TODO: TEMP
#include "services/InstanceService.h"
#include "services/PhysicalDeviceService.h"
#include "services/DeviceQueueService.h"
#include "services/SwapChainService.h"
#include "services/MemoryAllocationService.h"
