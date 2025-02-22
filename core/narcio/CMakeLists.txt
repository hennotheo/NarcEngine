set(NARC_IO "NarcIO" CACHE STRING "" FORCE)
set(NARC_IO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE STRING "" FORCE)

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_compile_definitions(NARC_IO_BUILD_DLL)

add_library(${NARC_IO} STATIC include/NarcIO.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

include_directories(${NARC_IO_INCLUDE})
include_directories("vendors/stb")

target_precompile_headers(${NARC_IO} PRIVATE "include/pch.h")

add_custom_command(TARGET ${NARC_IO} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NARC_IO}> ${SANDBOX_BIN_DIR}/$<TARGET_FILE_NAME:${NARC_IO}>
)