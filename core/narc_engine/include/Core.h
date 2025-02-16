#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#if NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API __declspec(dllexport)

#else
      
#define NARC_ENGINE_API __declspec(dllimport)
      
#endif


#ifdef NDEBUG

#else

#define ENABLE_VALIDATION_LAYERS

#endif