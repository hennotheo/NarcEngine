#pragma once

#include <vulkan/vulkan.h>

#include "DescriptorPoolBuilder.h"
#include "core/DeviceHandler.h"

namespace narc_engine {
    class DescriptorPool
    {
    public:
        void create(DescriptorPoolBuilder* builder);

        void allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets);

        void release();

    private:
        VkDescriptorPool m_descriptorPool;

        const DeviceHandler* m_deviceHandler;
    };
}
