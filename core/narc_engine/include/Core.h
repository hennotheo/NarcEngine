#pragma once

#if NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API __declspec(dllexport)

#else

#define NARC_ENGINE_API __declspec(dllimport)

#endif

#ifdef NDEBUG

// const bool EnableValidationLayers = false;

#else

#define ENABLE_VALIDATION_LAYERS
// const bool EnableValidationLayers = true;
#endif
