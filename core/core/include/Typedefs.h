//
// Created by theo on 3/12/26.
//


#pragma once

namespace narc_core {
    using result = bool;

    template<typename T>
    using injected_component = std::shared_ptr<T>;

    template<typename T>
    using injected_service = std::shared_ptr<T>;
}
