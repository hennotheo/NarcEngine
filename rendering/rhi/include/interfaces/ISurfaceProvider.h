//
// Created by theohenno on 12/4/25.
//

#pragma once

namespace narc_engine {
    class ISurfacesHandler : public IService
    {
    public:
        virtual ~ISurfacesHandler() override = default;

        NARC_PURE_VIRTUAL_GETTER(const IVulkanSurface*, getMainSurface);
    };
}
