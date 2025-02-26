set(NARC_LOG "NarcLog" CACHE STRING "" FORCE)
set(NARC_LOG_INCLUDE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
#        "${CMAKE_CURRENT_SOURCE_DIR}/vendors"
        CACHE STRING "" FORCE)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
add_library(${NARC_LOG} SHARED include/NarcLog.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(${NARC_LOG} PRIVATE NARC_LOG_BUILD_DLL)
target_precompile_headers(${NARC_LOG} PRIVATE "include/pch.h")

target_include_directories(${NARC_LOG} PRIVATE ${NARC_LOG_INCLUDE})