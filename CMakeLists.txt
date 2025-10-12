cmake_minimum_required(VERSION 3.31)
cmake_policy(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(scripts/versionning.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

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


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

add_subdirectory(core/core)
add_subdirectory(core/narc_math)


add_subdirectory(core/narclog)
return()
add_subdirectory(core/narcio)

add_subdirectory(rendering)

add_subdirectory(editor/application)

add_subdirectory(resources)

include(scripts/installation.cmake)