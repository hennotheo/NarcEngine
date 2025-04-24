cmake_minimum_required(VERSION 3.31)
cmake_policy(VERSION 3.31)

include(cmake/versionning.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release")
endif ()

project(NarcEngineEditor
    VERSION ${NARC_VERSION}
    LANGUAGES CXX C)

include(cmake/helpers.cmake)
include(cmake/dependencies.cmake)
include(cmake/tests.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_BUILD_DEBUG" APPEND)
endif()

set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_ENGINE_PLATFORM_WINDOWS" APPEND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

add_subdirectory(core/core)
add_subdirectory(core/narc_math)
add_subdirectory(core/narclog)
add_subdirectory(core/narcio)

add_subdirectory(rendering/shaders)
add_subdirectory(rendering/narc_engine)

add_subdirectory(application)

include(cmake/installation.cmake)
