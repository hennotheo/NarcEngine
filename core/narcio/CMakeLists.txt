set(NarcIO "NarcIO" CACHE STRING "" FORCE)

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_compile_definitions(${NarcIO}_BUILD_DLL)

add_library(${NarcIO} SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("vendors/stb")

target_precompile_headers(${NarcIO} PRIVATE "include/pch.h")

add_custom_command(TARGET ${NarcIO} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NarcIO}> ${SANDBOX_BIN_DIR}/$<TARGET_FILE_NAME:${NarcIO}>
)