//
// Created by theo on 2/13/26.
//

#pragma once

#ifdef NARC_ENGINE_PLATFORM_LINUX
#include <xcb/xcb.h>
#else
#error Not Implemented.
#endif

#include "Typedefs.h"

#include "ApplicationInfo.h"

#include "models/NativeWindowHandle.h"

#include "device/IVulkanExtension.h"
#include "device/PhysicalDeviceCriteria.h"
#include "device/QueueFamilyIndices.h"

#include "geometry/Vertex.h"

#include "interfaces/surface/ISurfaceHandler.h"
#include "interfaces/surface/IWindow.h"
#include "interfaces/IGraphicsInstance.h"
#include "interfaces/ICommandBuffer.h"
#include "interfaces/IFence.h"
#include "interfaces/IGraphicsPipeline.h"
#include "interfaces/IImage.h"
#include "interfaces/IPipelineLayout.h"
#include "interfaces/IQueue.h"
#include "interfaces/ISampler.h"
#include "interfaces/ISemaphore.h"
#include "interfaces/ISwapchain.h"
