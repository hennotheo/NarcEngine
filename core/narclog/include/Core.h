//
// Created by theoh on 26/02/2025.
//

#pragma once

#ifdef NARCLOG_BUILD_DLL

#define NARCLOG_API __declspec(dllexport)

#else

#define NARCLOG_API __declspec(dllimport)

#endif

#define NARCLOG_FILE_LINE __FILE__, __LINE__

typedef std::vector<const char*> KeywordList;