//
// Created by theo on 3/12/26.
//


#pragma once

namespace narc_core {
    template<typename T, typename U>
    const T* backend_cast(const U* base)
    {
        static_assert(std::is_base_of_v<U, T>);
        if (base == nullptr)
        {
            return nullptr;
        }
#ifdef NARC_BUILD_DEBUG
        const auto* derived = dynamic_cast<const T*>(base);
        assert(derived && "Backend type mismatch");
        return derived;
#else
        return static_cast<const T*>(base);
#endif
    }

    template<template<typename...> class Container>
    struct to
    {
        // Tag
    };

    template<std::ranges::input_range R, template<typename...> class Container>
    auto operator|(R&& range, to<Container> const&)
    {
        using T = std::ranges::range_value_t<R>;
        return Container<T>(std::begin(range), std::end(range));
    }

    template<typename T>
    struct transform_to_concrete_class
    {
    };

    //TO
    template<std::ranges::input_range R, typename T>
    auto operator|(R&& range, transform_to_concrete_class<T> const&)
    {
        return std::forward<R>(range)
               | std::views::transform([](auto* cmdBuffer) noexcept {
                   return backend_cast<T>(cmdBuffer);
               })
               | std::views::filter([](auto* ptr) noexcept {
                   return ptr != nullptr;
               });
    }
}
