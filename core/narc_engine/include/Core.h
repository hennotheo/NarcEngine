#pragma once

#include <vector>

#if NARC_ENGINE_BUILD_DLL

#define NARC_ENGINE_API __declspec(dllexport)

#else

#define NARC_ENGINE_API __declspec(dllimport)

#endif

#ifdef NDEBUG

// const bool EnableValidationLayers = false;

#else

#define ENABLE_VALIDATION_LAYERS
const std::vector<const char*> ValidationLayers =
{
    "VK_LAYER_KHRONOS_validation"
};


// const bool EnableValidationLayers = true;
#endif
