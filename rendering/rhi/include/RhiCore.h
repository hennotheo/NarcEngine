#pragma once

#ifdef NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API NARC_DLL_EXPORT

#elif defined(TESTING_BUILD_DLL)

#define NARC_ENGINE_API

#else

#define NARC_ENGINE_API NARC_DLL_IMPORT

#endif


#ifdef NARC_BUILD_DEBUG

#define ENABLE_VALIDATION_LAYERS

#endif

using VulkanMemory = void;