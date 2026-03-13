if (ENABLE_TESTS)

    add_compile_definitions(NARC_TEST_BUILD)
    enable_testing()

    find_package(Catch2 REQUIRED)

endif ()

function(narc_auto_glob_tests)
    file(GLOB_RECURSE TESTS_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    file(GLOB_RECURSE TESTS_HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)

    set(TESTS_CPP_FILES ${TESTS_CPP_FILES} PARENT_SCOPE)
    set(TESTS_CPP_FILES ${TESTS_CPP_FILES} PARENT_SCOPE)
endfunction()

function(narc_auto_setup_tests TARGET_NAME)
    narc_is_target_testable(IS_TESTABLE)

    if(NOT ENABLE_TESTS OR NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        return()
    endif()

    add_executable(Tests_${TARGET_NAME} tests/test_main.cpp)

    target_link_libraries(Tests_${TARGET_NAME} PRIVATE
            gtest gtest_main
            gmock gmock_main
            ${TARGET_NAME})

    gtest_discover_tests(Tests_${TARGET_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endfunction()

function(narc_decl_testable_target TARGET_NAME)
    if (NOT ENABLE_TESTS OR NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        return()
    endif ()

    narc_auto_glob_tests()
    link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})
    add_executable(Tests_${TARGET_NAME} tests/test_main.cpp ${TESTS_CPP_FILES} ${TESTS_HPP_FILES} ${CPP_FILES} ${HPP_FILES})
    add_dependencies(Tests_${TARGET_NAME} ${TARGET_NAME})
    add_compile_definitions(Tests_${TARGET_NAME} PRIVATE NARC_TEST_BUILD)
    target_include_directories(Tests_${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/tests")
#    target_compile_features(Tests_${TARGET_NAME} PRIVATE cxx_std_20)

    target_link_libraries(Tests_${TARGET_NAME} PRIVATE
            Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(Tests_${TARGET_NAME})

    #    gtest_discover_tests(Tests_${TARGET_NAME}
    #            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endfunction()

function(narc_testable_add_dependencies TARGET_NAME)
    add_dependencies(${TARGET_NAME} ${ARGN})

    if (ENABLE_TESTS AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        add_dependencies(Tests_${TARGET_NAME} ${ARGN})
    endif ()
endfunction()

function(narc_testable_add_compile_definition TARGET_NAME PROTECTION)
    target_compile_definitions(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        target_compile_definitions(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_add_include_directory TARGET_NAME PROTECTION)
    target_include_directories(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        target_include_directories(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_link_library TARGET_NAME PROTECTION)
    target_link_libraries(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        target_link_libraries(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_pch TARGET_NAME PROTECTION)
    target_precompile_headers(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        target_precompile_headers(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_target_sources TARGET_NAME PROTECTION)
    target_sources(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        target_sources(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()