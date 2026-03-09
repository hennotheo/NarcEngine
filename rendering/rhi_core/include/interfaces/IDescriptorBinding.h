//
// Created by theo on 3/5/26.
//

#pragma once

namespace narc_engine {
    class IImage;
    class IBuffer;

    class IDescriptorBindingUpdater
    {
    public:
        virtual ~IDescriptorBindingUpdater() = default;

        virtual void updateBuffer(uint32_t bindingIndex, const IBuffer* buffer) = 0;
        virtual void updateImageSampler(uint32_t bindingIndex, const IImage* image)= 0;
        virtual void update()= 0;
    };

    class IDescriptorBinding
    {
    public:
        virtual ~IDescriptorBinding() = default;

        NARC_PURE_VIRTUAL_GETTER(const IDescriptorLayout*, getLayout);

        virtual std::unique_ptr<IDescriptorBindingUpdater> createUpdater() = 0;
    };
}