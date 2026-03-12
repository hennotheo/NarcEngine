//
// Created by theo on 3/12/26.
//

#pragma once

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