if (ENABLE_COVERAGE AND NOT ENABLE_TESTS)
    message(FATAL_ERROR "ENABLE_COVERAGE requires ENABLE_TESTS to be ON.")
endif ()

if (ENABLE_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Building with coverage enabled")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else ()
        message(FATAL_ERROR "Coverage is only supported with GCC or Clang.")
    endif ()
endif ()

if (ENABLE_TESTS)
    add_compile_definitions(NARC_TEST_BUILD)
    include(CTest)
    enable_testing()
    find_package(Catch2 REQUIRED)
    include(Catch)
endif ()

if (ENABLE_COVERAGE)
    find_program(LCOV_EXEC lcov)
    find_program(GENHTML_EXEC genhtml)

    if (NOT LCOV_EXEC)
        message(FATAL_ERROR "lcov not found")
    endif ()

    if (NOT GENHTML_EXEC)
        message(FATAL_ERROR "genhtml not found")
    endif ()

    add_custom_target(NarcCoverage
            COMMAND ${LCOV_EXEC} --directory ${CMAKE_BINARY_DIR} --zerocounters
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${LCOV_EXEC}
            --directory ${CMAKE_BINARY_DIR}
            --capture
            --output-file coverage.info
            COMMAND ${LCOV_EXEC}
            --remove coverage.info '/usr/*' '*/.conan2/*' '*/tests/*'
            --output-file coverage.info
            COMMAND ${GENHTML_EXEC}
            coverage.info
            --output-directory coverage-report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running tests and generating coverage report"
    )
endif ()

function(narc_auto_glob_tests)
    file(GLOB_RECURSE TESTS_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    file(GLOB_RECURSE TESTS_HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)

    set(TESTS_CPP_FILES ${TESTS_CPP_FILES} PARENT_SCOPE)
    set(TESTS_HPP_FILES ${TESTS_HPP_FILES} PARENT_SCOPE)
endfunction()

function(narc_decl_testable_target TARGET_NAME)
    if (NOT ENABLE_TESTS OR NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        return()
    endif ()

    narc_auto_glob_tests()
    list(REMOVE_ITEM TESTS_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_main.cpp")
    add_executable(Tests_${TARGET_NAME} tests/test_main.cpp ${TESTS_CPP_FILES} ${TESTS_HPP_FILES} ${CPP_FILES} ${HPP_FILES})
    add_dependencies(Tests_${TARGET_NAME} ${TARGET_NAME})
    target_compile_definitions(Tests_${TARGET_NAME} PRIVATE NARC_TEST_BUILD)
    target_include_directories(Tests_${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/tests")

    target_link_libraries(Tests_${TARGET_NAME} PRIVATE
            Catch2::Catch2WithMain
    )

    catch_discover_tests(Tests_${TARGET_NAME})
endfunction()

function(narc_testable_add_dependencies TARGET_NAME)
    add_dependencies(${TARGET_NAME} ${ARGN})

    if (ENABLE_TESTS AND TARGET Tests_${TARGET_NAME})
        add_dependencies(Tests_${TARGET_NAME} ${ARGN})
    endif ()
endfunction()

function(narc_testable_add_compile_definition TARGET_NAME PROTECTION)
    target_compile_definitions(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND TARGET Tests_${TARGET_NAME})
        target_compile_definitions(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_add_include_directory TARGET_NAME PROTECTION)
    target_include_directories(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND TARGET Tests_${TARGET_NAME})
        target_include_directories(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_link_library TARGET_NAME PROTECTION)
    target_link_libraries(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND TARGET Tests_${TARGET_NAME})
        target_link_libraries(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_pch TARGET_NAME PROTECTION)
    target_precompile_headers(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND TARGET Tests_${TARGET_NAME})
        target_precompile_headers(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()

function(narc_testable_target_sources TARGET_NAME PROTECTION)
    target_sources(${TARGET_NAME} ${PROTECTION} ${ARGN})

    if (ENABLE_TESTS AND TARGET Tests_${TARGET_NAME})
        target_sources(Tests_${TARGET_NAME} ${PROTECTION} ${ARGN})
    endif ()
endfunction()
