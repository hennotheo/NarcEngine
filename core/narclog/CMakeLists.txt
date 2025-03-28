file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(NarcLog SHARED include/NarcLog.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(NarcLog PRIVATE NARCLOG_BUILD_DLL)
target_precompile_headers(NarcLog PRIVATE "include/pch.h")

add_dependencies(NarcLog NarcCore)

target_include_directories(NarcLog PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(NarcLog PUBLIC NarcCore)

#----------------
#----- TEST -----
#----------------

add_executable(NarcLog_Tests tests/test_main.cpp ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcLog_Tests
        googleTest
        NarcCore
        NarcLog)

target_link_directories(NarcLog_Tests PRIVATE
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        "${VENDORS_DIR}/lib")

target_compile_definitions(NarcLog_Tests PRIVATE
        NARCLOG_BUILD_DLL)

target_include_directories(NarcLog_Tests PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${VENDORS_DIR}/include")

target_link_libraries(NarcLog_Tests PRIVATE
        NarcCore
        gtest gtest_main
        gmock gmock_main)

target_precompile_headers(NarcLog_Tests PRIVATE "include/pch.h")