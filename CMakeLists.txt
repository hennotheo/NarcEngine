cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
set (CONFIGURATION "Debug")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SANDBOX "Sandbox")
set(SANDBOX_BIN_DIR ${CMAKE_BINARY_DIR}/${SANDBOX}/${CONFIGURATION})

if (NOT EXISTS ${SANDBOX_BIN_DIR})
    file(MAKE_DIRECTORY ${SANDBOX_BIN_DIR})
endif()

project (NarcEngine VERSION 0.1)

add_compile_definitions(NARC_ENGINE_PLATFORM_WINDOWS)

add_subdirectory (NarcEngine)
add_subdirectory (Sandbox)