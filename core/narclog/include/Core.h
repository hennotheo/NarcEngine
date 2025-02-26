//
// Created by theoh on 26/02/2025.
//

#pragma once

#ifdef NARC_LOG_BUILD_DLL

#define NARC_LOG_API __declspec(dllexport)

#else

#define NARC_LOG_API __declspec(dllimport)

#endif