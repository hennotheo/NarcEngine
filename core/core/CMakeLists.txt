file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(NarcCore STATIC include/NarcCore.h ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(NarcCore PRIVATE NARC_CORE_BUILD_DLL)

target_include_directories(NarcCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_precompile_headers(NarcCore PUBLIC "include/pch.h")