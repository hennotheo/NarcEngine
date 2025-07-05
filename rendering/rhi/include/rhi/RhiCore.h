#pragma once

#ifdef NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API __declspec(dllexport)
#define _internal public
#define _internal_protected public

#elif defined(TESTING_BUILD_DLL)

#define NARC_ENGINE_API

#else

#define NARC_ENGINE_API __declspec(dllimport)
#define _internal private
#define _internal_protected protected

#endif


#ifdef NARC_BUILD_DEBUG

#define ENABLE_VALIDATION_LAYERS

#endif

using RhiResult = bool;
#define RHI_SUCCESS true
#define RHI_FAILURE false