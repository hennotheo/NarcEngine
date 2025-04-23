#pragma once

#ifdef NARC_MATH_BUILD_DLL

#define NARC_ENGINE_API __declspec(dllexport)

#else

#define NARC_ENGINE_API __declspec(dllimport)

#endif