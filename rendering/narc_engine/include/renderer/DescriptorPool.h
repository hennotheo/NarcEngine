#pragma once

#include <vulkan/vulkan.h>

#include "DescriptorPoolBuilder.h"
#include "core/DeviceHandler.h"

namespace narc_engine {
    class DescriptorPool
    {
    public:
        explicit DescriptorPool(uint32_t poolCount);
        ~DescriptorPool();

        GETTER DescriptorPoolBuilder* builder() const { return m_builder.get(); }

        void create();

        void allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets) const;

        void release();

    private:
        std::unique_ptr<DescriptorPoolBuilder> m_builder;

        VkDescriptorPool m_descriptorPool;

        bool m_allocated = false;

        const DeviceHandler* m_deviceHandler;
    };
}
