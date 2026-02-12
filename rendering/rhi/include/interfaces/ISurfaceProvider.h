//
// Created by theohenno on 12/4/25.
//

#pragma once

namespace narc_engine {
    class ISurfacesHandler : public IService
    {
    public:
        ~ISurfacesHandler() override = default;

        NARC_PURE_VIRTUAL_GETTER(const IVulkanSurface*, getMainSurface);
    };
}
