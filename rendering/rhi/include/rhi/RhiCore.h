#pragma once

#ifdef NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API NARC_DLL_EXPORT
#define _internal public
#define _internal_protected public

#elif defined(TESTING_BUILD_DLL)

#define NARC_ENGINE_API

#else

#define NARC_ENGINE_API NARC_DLL_IMPORT
#define _internal private
#define _internal_protected protected

#endif


#ifdef NARC_BUILD_DEBUG

#define ENABLE_VALIDATION_LAYERS

#endif

using RhiResult = bool;
#define RHI_SUCCESS true
#define RHI_FAILURE false

///TODO: Change this LATER
typedef std::vector<const char*> KeywordList;