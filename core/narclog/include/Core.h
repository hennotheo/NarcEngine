//
// Created by theoh on 26/02/2025.
//

#pragma once

#ifdef NARC_LOG_BUILD_DLL

#define NARC_LOG_API NARC_DLL_EXPORT

#else

#define NARC_LOG_API NARC_DLL_IMPORT

#endif

#define NARC_LOG_FILE_LINE __FILE__, __LINE__