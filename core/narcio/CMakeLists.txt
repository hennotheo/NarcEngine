include(ExternalProject)
include(FetchContent)
ExternalProject_Add(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v2.0.0rc13
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors/stb
)
FetchContent_MakeAvailable(stb)

set(NARC_IO "NarcIO" CACHE STRING "" FORCE)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
add_library(${NARC_IO} SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

add_dependencies(${NARC_IO}
        ${NARC_LOG}
        ${NARC_MATH}
        tinyobjloader)

target_compile_definitions(${NARC_IO} PRIVATE NARC_IO_BUILD_DLL)
target_precompile_headers(${NARC_IO} PRIVATE "include/pch.h")

target_include_directories(${NARC_IO} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${NARC_IO} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
        "${stb_SOURCE_DIR}"
)


target_link_libraries(${NARC_IO} PUBLIC ${NARC_LOG} ${NARC_MATH})