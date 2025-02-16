cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()

set(SANDBOX "Sandbox")
set(SANDBOX_BIN_DIR ${CMAKE_BINARY_DIR}/${SANDBOX}/${CMAKE_BUILD_TYPE})

set(VULKAN_GLSLC_PATH "" CACHE INTERNAL "Vulkan GLSL Compiler Path")

if(NOT EXISTS ${SANDBOX_BIN_DIR})
file(MAKE_DIRECTORY ${SANDBOX_BIN_DIR})
endif()

project(NarcEngine VERSION 0.1)

add_compile_definitions(NARC_ENGINE_PLATFORM_WINDOWS)

add_subdirectory(core/narc_engine)
add_subdirectory(core/shaders)
add_subdirectory(sandbox)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)