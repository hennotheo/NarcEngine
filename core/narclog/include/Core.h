//
// Created by theoh on 26/02/2025.
//

#pragma once

#ifdef NARCLOG_BUILD_DLL

#define NARCLOG_API NARC_DLL_EXPORT

#else

#define NARCLOG_API NARC_DLL_IMPORT

#endif

#define NARCLOG_FILE_LINE __FILE__, __LINE__

typedef std::vector<const char*> KeywordList;