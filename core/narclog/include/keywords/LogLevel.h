//
// Created by theoh on 26/02/2025.
//
#pragma once

///@brief Log levels for NarcLog
/// @note DEBUG level only appears in debug mode, all other levels appear in release mode
enum LogLevel
{
    FATAL, //Appears in release mode
    ERROR, //Appears in release mode
    WARNING, //Appears in release mode
    INFO, //Appears in release mode
    DEBUG //Only in debug mode
};