cmake_minimum_required(VERSION 3.31)
cmake_policy(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(scripts/versionning.cmake)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13.0")
        message(FATAL_ERROR "GCC 13+ required for C++23. Current: ${CMAKE_CXX_COMPILER_VERSION}")
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16.0")
        message(FATAL_ERROR "Clang 16+ required for C++23. Current: ${CMAKE_CXX_COMPILER_VERSION}")
    endif ()
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++23")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(NarcEngineEditor
        VERSION ${NARC_VERSION}
        LANGUAGES CXX)

include(scripts/helpers.cmake)
include(scripts/dependencies.cmake)
include(scripts/tests.cmake)

narc_set_shared_lib()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_BUILD_DEBUG" APPEND)
endif ()

if (WIN32)
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_ENGINE_PLATFORM_WINDOWS" APPEND)
elseif (UNIX AND NOT APPLE)
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_ENGINE_PLATFORM_LINUX" APPEND)
elseif (APPLE)
    message(FATAL_ERROR "Apple platform is not supported yet.")
endif ()

add_subdirectory(core/core)
add_subdirectory(core/narc_math)


add_subdirectory(core/narclog)
add_subdirectory(core/narcio)

add_subdirectory(rendering)

add_subdirectory(editor/application)

add_subdirectory(resources)

include(scripts/installation.cmake)