#pragma once

#include <vulkan/vulkan.h>

#include "renderer/DescriptorPoolBuilder.h"
#include "core/DeviceComponent.h"
#include "core/devices/DeviceHandler.h"

namespace narc_engine {
    class DescriptorPool : public DeviceComponent, public narc_core::IGetter<VkDescriptorPool>
    {
    public:
        explicit DescriptorPool(uint32_t poolCount);
        ~DescriptorPool();

        NARC_IMPL_IGETTER(VkDescriptorPool, m_descriptorPool)
        GETTER DescriptorPoolBuilder* builder() const { return m_builder.get(); }

        void create();

        void allocateDescriptorSets(VkDescriptorSetAllocateInfo* allocInfo, VkDescriptorSet* descriptorSets) const;

        void release();

    private:
        std::unique_ptr<DescriptorPoolBuilder> m_builder;

        VkDescriptorPool m_descriptorPool;

        bool m_allocated = false;
    };
}
