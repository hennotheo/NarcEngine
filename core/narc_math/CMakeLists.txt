include(ExternalProject)
ExternalProject_Add(
        glm
        GIT_REPOSITORY
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

set(NARC_MATH "NarcMath" CACHE STRING "" FORCE)
set(NARC_MATH_INCLUDE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
        CACHE STRING "" FORCE)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
add_library(${NARC_MATH} SHARED include/NarcMath.h ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(${NARC_MATH} PRIVATE NARC_MATH_BUILD_DLL)
target_precompile_headers(${NARC_MATH} PRIVATE "include/pch.h")

add_dependencies(${NARC_MATH} glm)
target_include_directories(${NARC_MATH} PRIVATE ${NARC_MATH_INCLUDE})