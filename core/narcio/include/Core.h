#pragma once

#if NarcIO_BUILD_DLL

#define NarcIO_API __declspec(dllexport)

#else

#define NarcIO_API __declspec(dllimport)

#endif