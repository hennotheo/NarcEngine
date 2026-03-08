//
// Created by theoh on 3/10/2025.
//

#pragma once

#define NO_DISCARD [[nodiscard]]
#define NO_RETURN [[noreturn]]
#define DEPRECATED [[deprecated("Deprecated. This code will be removed in the future.")]]
#define TEMP_CODE [[deprecated("Temporary code just for testing purposes")]]

#define NARC_MUTABLE_THIS(type) const_cast<type*>(this)


#ifdef NARC_TEST_BUILD

#define PRIVATE_TESTABLE public
#define PROTECTED_TESTABLE public

#else

#define PRIVATE_TESTABLE private
#define PROTECTED_TESTABLE protected

#endif

#include "pch.h"

#include "macros/Format.h"
#include "macros/GetterSetter.h"
#include "macros/Debug.h"

#include "Events.h"

#include "interfaces/IService.h"
#include "interfaces/IGetter.h"
#include "interfaces/IInitialisable.h"
#include "interfaces/ILoadable.h"

#include "utils/UtilsFunctions.h"

#define NARC_GUARD_RAW_PTR(ptr, errorMsg)                                                                                                                                                                                                        \
    if (ptr == nullptr)                                                                                                                                  \
    {                                                                                                                                                \
        NARC_ERROR_RUNTIME(errorMsg);                                                                                                                \
    }

#define NARC_GUARD_WEAK(varName, weakPtr, errorMsg)                                                                                                  \
    const auto varName = (weakPtr).lock();                                                                                                           \
    if (!(varName))                                                                                                                                  \
    {                                                                                                                                                \
        NARC_ERROR_RUNTIME(errorMsg);                                                                                                                \
    }

#define NARC_GUARD_WEAK_UNEXPECTED(varName, weakPtr, errorMsg)                                                                                       \
    const auto varName = (weakPtr).lock();                                                                                                           \
    if (!(varName))                                                                                                                                  \
    {                                                                                                                                                \
        std::unexpected(std::string{errorMsg});                                                                                                 \
    }

#if defined(NARC_ENGINE_PLATFORM_WINDOWS)
#define NARC_DLL_EXPORT __declspec(dllexport)
#define NARC_DLL_IMPORT __declspec(dllimport)

#elif defined(NARC_ENGINE_PLATFORM_LINUX)
#define NARC_DLL_EXPORT __attribute__((visibility("default")))
#define NARC_DLL_IMPORT __attribute__((visibility("default")))

#else
#define NARC_DLL_EXPORT
#define NARC_DLL_IMPORT

#endif

namespace di = boost::di;

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

#define NARC_DI_SERVICE_NAME(type) type##Injected

    template<typename T>
    using injected_service = std::shared_ptr<T>;
#define NARC_DI_IMPORT_SERVICE(type) narc_core::injected_service<type> NARC_DI_SERVICE_NAME(type)
#define NARC_DI_IMPL_SERVICE(type, property) property(std::move(NARC_DI_SERVICE_NAME(type)))

    template<typename T>
    using injected_component = std::shared_ptr<T>;
#define NARC_DI_IMPORT_COMPONENT(type) narc_core::injected_component<type> NARC_DI_SERVICE_NAME(type)
#define NARC_DI_IMPL_COMPONENT(type, property) property(std::move(NARC_DI_SERVICE_NAME(type)))

    using result = bool;

    template<typename T>
    class ICreator
    {
    public:
        virtual ~ICreator() = default;

        NO_DISCARD virtual std::unique_ptr<T> create() const noexcept = 0;
    };

    template<template<typename...> class Container>
    struct to
    {
        // Tag
    };

    // | operator
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

    // | operator
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

#define QUERY(result, error) DEPRECATED NO_DISCARD std::expected<result, error>
#define NARC_VIRTUAL_QUERY(type, displayName, ...) NO_DISCARD virtual type displayName(__VA_ARGS__) const noexcept
#define NARC_PURE_VIRTUAL_QUERY(type, displayName, ...) NO_DISCARD virtual type displayName(__VA_ARGS__) const noexcept = 0
#define NARC_QUERY_OVERRIDE(type, displayName, ...) NO_DISCARD virtual type displayName(__VA_ARGS__) const noexcept override
#define NARC_QUERY(type, displayName, ...) NO_DISCARD type displayName(__VA_ARGS__) const noexcept

//TODO: REMOVE THIS OR CONVERT TO
#define NARC_VIRTUAL_CMD(displayName, ...) virtual narc_core::result displayName(__VA_ARGS__) const noexcept
#define NARC_PURE_VIRTUAL_CMD(displayName, ...) virtual narc_core::result displayName(__VA_ARGS__) const noexcept = 0
#define NARC_CMD_OVERRIDE(displayName, ...) narc_core::result displayName(__VA_ARGS__) const noexcept override

#define NARC_MAP(typeA, typeB) \
NO_DISCARD typeB map##typeA##To##typeB(const typeA& value) const noexcept
#define NARC_PURE_VIRTUAL_MAP(typeA, typeB) \
NO_DISCARD virtual typeB map##typeA##To##typeB(const typeA& value) const noexcept = 0
#define NARC_MAP_OVERRIDE(typeA, typeB) \
NO_DISCARD typeB map##typeA##To##typeB(const typeA& value) const noexcept override
