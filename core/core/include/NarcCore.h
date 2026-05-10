//
// Created by theoh on 3/10/2025.
//

#pragma once

#define NO_DISCARD [[nodiscard]]
#define NO_RETURN [[noreturn]]
#define DEPRECATED [[deprecated("Deprecated. This code will be removed in the future.")]]
#define TEMP_CODE [[deprecated("Temporary code just for testing purposes")]]

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
#include "macros/Dll.h"

#include "Typedefs.h"

#include "interfaces/IService.h"
#include "interfaces/IGetter.h"
#include "interfaces/IInitialisable.h"
#include "interfaces/ILoadable.h"

#include "Helpers.h"


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
