file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(NarcLog SHARED include/NarcLog.h ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(NarcLog PRIVATE NARCLOG_BUILD_DLL)
target_precompile_headers(NarcLog PRIVATE "include/pch.h")

add_dependencies(NarcLog NarcCore)

target_include_directories(NarcLog PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(NarcLog PUBLIC NarcCore)

#----------------
#----- TEST -----
#----------------

# add_executable(Tests_NarcLog tests/test_main.cpp ${CPP_FILES} ${HPP_FILES})

# add_dependencies(Tests_NarcLog
#         NarcCore
#         NarcLog)

# target_link_directories(Tests_NarcLog PRIVATE
#         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#         "${VENDORS_DIR}/lib")

# target_compile_definitions(Tests_NarcLog PRIVATE
#         NARCLOG_BUILD_DLL)

# target_include_directories(Tests_NarcLog PUBLIC
#         "${CMAKE_CURRENT_SOURCE_DIR}/include"
#         "${VENDORS_DIR}/include")

# target_link_libraries(Tests_NarcLog PRIVATE
#         NarcCore
#         gtest gtest_main
#         gmock gmock_main)

# target_precompile_headers(Tests_NarcLog PRIVATE "include/pch.h")