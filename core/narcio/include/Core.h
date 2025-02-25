#pragma once

#ifdef NARC_IO_BUILD_DLL

#define NARC_IO_API __declspec(dllexport)

#else

#define NARC_IO_API __declspec(dllimport)

#endif