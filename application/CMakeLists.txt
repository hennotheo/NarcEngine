file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_executable(NarcEngineEditor src/main.cpp ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngineEditor
        NarcEngine)

target_include_directories(NarcEngineEditor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(NarcEngineEditor PRIVATE NarcEngine)

#----------------
#----- TEST -----
#----------------

set(TEST_CPP_FILES ${CPP_FILES})
list(REMOVE_ITEM TEST_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_executable(NarcEngineEditor_Tests tests/test_main.cpp ${TEST_CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngineEditor
        googleTest
        NarcEngine)

target_link_directories(NarcEngineEditor_Tests PRIVATE
        ${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE}
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        "${VENDORS_DIR}/lib")

target_include_directories(NarcEngineEditor_Tests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
        "${VENDORS_DIR}/include")

target_link_libraries(NarcEngineEditor_Tests PRIVATE
        NarcEngine
        gtest gtest_main
        gmock gmock_main)