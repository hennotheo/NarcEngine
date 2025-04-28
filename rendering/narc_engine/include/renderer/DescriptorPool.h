#pragma once

#include <vulkan/vulkan.h>

#include "DescriptorPoolBuilder.h"
#include "core/DeviceComponent.h"
#include "core/devices/DeviceHandler.h"

namespace narc_engine {
    class DescriptorPool : public DeviceComponent
    {
    public:
        explicit DescriptorPool(uint32_t poolCount);
        ~DescriptorPool();

        GETTER DescriptorPoolBuilder* builder() const { return m_builder.get(); }
        GETTER VkDescriptorPool getVkDescriptorPool() const { return m_descriptorPool; }

        void create();

        void allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets) const;

        void release();

    private:
        std::unique_ptr<DescriptorPoolBuilder> m_builder;

        VkDescriptorPool m_descriptorPool;

        bool m_allocated = false;
    };
}
