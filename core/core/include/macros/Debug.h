//
// Created by theohenno on 10/13/25.
//

#pragma once

/// Debug only property, removed in release builds
#define NARC_DEBUG_PROPERTY(type, name, initialValue) NARC_DEBUG_ONLY(type name = initialValue)

#if defined(NARC_BUILD_DEBUG)

/// Debug only code, removed in release builds
#define NARC_DEBUG_ONLY(...) __VA_ARGS__

#else

/// Debug only property, removed in release builds
#define NARC_DEBUG_ONLY(...)

#endif
