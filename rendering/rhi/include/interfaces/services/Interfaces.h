//
// Created by theohenno on 12/2/25.
//

#pragma once

namespace narc_engine {
    using VulkanCreationQueryError = std::string;
    
    template<typename T>
    using VulkanCreationQuery = std::expected<T, VulkanCreationQueryError>;
    
    class IService
    {
    public:
        virtual ~IService() = default;
    };
}

#include "IInstanceService.h"