set(NARC_IO "NarcIO" CACHE STRING "" FORCE)
set(NARC_IO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING "" FORCE)

add_compile_definitions(NARC_IO_BUILD_DLL)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
add_library(${NARC_IO} SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

target_precompile_headers(${NARC_IO} PRIVATE "include/pch.h")

target_include_directories(${NARC_IO} PRIVATE ${NARC_IO_INCLUDE} "vendors/stb")