//
// Created by theoh on 3/10/2025.
//

#pragma once

#define QUERY [[nodiscard]]

#define NARC_MUTABLE_THIS(type) const_cast<type*>(this)

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

#include "Events.h"

#include "interfaces/IGetter.h"
#include "interfaces/IInitialisable.h"
#include "interfaces/ILoadable.h"

#define NARC_GUARD_WEAK(varName, weakPtr, errorMsg)                                                                                                  \
    const auto varName = (weakPtr).lock();                                                                                                           \
    if (!(varName))                                                                                                                                  \
    {                                                                                                                                                \
        NARC_ERROR_RUNTIME(errorMsg);                                                                                                                \
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
