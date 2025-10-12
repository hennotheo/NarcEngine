#pragma once

#ifdef NARC_MATH_BUILD_DLL

#define NARC_ENGINE_API NARC_DLL_EXPORT

#else

#define NARC_ENGINE_API NARC_DLL_IMPORT

#endif