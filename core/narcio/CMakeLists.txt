include(ExternalProject)
ExternalProject_Add(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v2.0.0rc13
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

ExternalProject_Add(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors/include/stb
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
)

set(NARC_IO "NarcIO" CACHE STRING "" FORCE)
set(NARC_IO_INCLUDE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
        ${NARC_LOG_INCLUDE}
        ${NARC_MATH_INCLUDE}
        CACHE STRING "" FORCE)

add_compile_definitions(NARC_IO_BUILD_DLL)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
add_library(${NARC_IO} SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

target_precompile_headers(${NARC_IO} PRIVATE "include/pch.h")

target_include_directories(${NARC_IO} PRIVATE ${NARC_IO_INCLUDE})

add_dependencies(${NARC_IO} ${NARC_LOG} ${NARC_MATH} tinyobjloader stb)

target_link_libraries(${NARC_IO} PUBLIC ${NARC_LOG} ${NARC_MATH})