include(ExternalProject)
ExternalProject_Add(
        googleTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.x
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_GMOCK=OFF
)

set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR}/bin/tests)

add_executable(Tests tests/test_main.cpp)

add_dependencies(Tests googleTest)

target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendors/include)

target_link_directories(Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendors/lib)

target_link_libraries(Tests PRIVATE gtest gtest_main)

set_target_properties(Tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

