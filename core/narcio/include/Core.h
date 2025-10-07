#pragma once

#ifdef NARC_IO_BUILD_DLL

#define NARC_IO_API NARC_DLL_EXPORT

#else

#define NARC_IO_API NARC_DLL_IMPORT

#endif