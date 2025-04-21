# ExternalProject_Add(
#         tinyobjloader
#         GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
#         GIT_TAG v2.0.0rc13
#         PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
#         CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
# )

set(NARCIO_DEPENDENCIES
        NarcLog
        NarcMath)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(NarcIO SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcIO ${NARCIO_DEPENDENCIES})

target_compile_definitions(NarcIO PRIVATE NARC_IO_BUILD_DLL)
target_precompile_headers(NarcIO PRIVATE "include/pch.h")

target_include_directories(NarcIO PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(NarcIO PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
        "${stb_SOURCE_DIR}"
        "${tinyobjloader_SOURCE_DIR}"
)

target_link_libraries(NarcIO PUBLIC NarcLog NarcMath)

#----------------
#----- TEST -----
#----------------

# add_executable(NarcIO_Tests tests/test_main.cpp ${CPP_FILES} ${HPP_FILES} src/FileReader.cpp)

# add_dependencies(NarcIO_Tests
#         googleTest
#         ${NARCIO_DEPENDENCIES})

# target_link_directories(NarcIO_Tests PRIVATE
#         ${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE}
#         ${CMAKE_CURRENT_SOURCE_DIR}/vendors/lib
#         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#         "${VENDORS_DIR}/lib")

# target_compile_definitions(NarcIO_Tests PRIVATE NARC_IO_BUILD_DLL)

# target_include_directories(NarcIO_Tests PRIVATE
#         "${CMAKE_CURRENT_SOURCE_DIR}/include"
#         "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
#         "${stb_SOURCE_DIR}"
#         "${VENDORS_DIR}/include")

# target_link_libraries(NarcIO_Tests PRIVATE
#         NarcLog
#         NarcMath
#         gtest gtest_main
#         gmock gmock_main)

# target_precompile_headers(NarcIO_Tests PRIVATE "include/pch.h")