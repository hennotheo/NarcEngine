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

if(ENABLE_TESTS)
        file(GLOB_RECURSE TESTS_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
        file(GLOB_RECURSE TESTS_HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)

        link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})
        add_executable(Tests_NarcEngineEditor tests/test_main.cpp ${TESTS_CPP_FILES} ${TESTS_HPP_FILES} ${CPP_FILES} ${HPP_FILES})

        add_dependencies(Tests_NarcEngineEditor NarcEngineEditor NarcEngine)

        target_include_directories(Tests_NarcEngineEditor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
        target_compile_definitions(Tests_NarcEngineEditor PRIVATE TESTING_BUILD_DLL)
        target_link_libraries(Tests_NarcEngineEditor PRIVATE
                gtest gtest_main
                gmock gmock_main
                NarcEngine)

        gtest_discover_tests(Tests_NarcEngineEditor
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()