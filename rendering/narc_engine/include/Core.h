#pragma once

#ifdef NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API __declspec(dllexport)


#elif defined(TESTING_BUILD_DLL)

#define NARC_ENGINE_API 

#else
      
#define NARC_ENGINE_API __declspec(dllimport)
      
#endif


#ifdef NARC_BUILD_DEBUG

#define ENABLE_VALIDATION_LAYERS

#else

#endif
