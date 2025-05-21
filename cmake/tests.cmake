option(ENABLE_TESTS "Enable Tests" OFF)

if(ENABLE_TESTS)

    add_compile_definitions(NARC_TEST_BUILD)
    enable_testing()

    # --- GOOGLE_TEST ---
    FetchContent_Declare(
    googleTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.x
    SOURCE_DIR ${VENDOR_DIR}/googleTests
)
    FetchContent_MakeAvailable(googleTest)
    include(GoogleTest)

endif()

function(narc_auto_glob_tests)
    file(GLOB_RECURSE TESTS_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    file(GLOB_RECURSE TESTS_HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)

    set(TESTS_CPP_FILES ${TESTS_CPP_FILES} PARENT_SCOPE)
    set(TESTS_CPP_FILES ${TESTS_CPP_FILES} PARENT_SCOPE)
endfunction()

function(narc_auto_setup_tests TARGET_NAME)
    if(NOT ENABLE_TESTS)
        return()
    endif()

    add_executable(Tests_${TARGET_NAME} tests/test_main.cpp)

    target_link_libraries(Tests_${TARGET_NAME} PRIVATE
        gtest gtest_main
        gmock gmock_main
        ${TARGET_NAME}
    )
    
    gtest_discover_tests(Tests_${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endfunction()

function(narc_decl_testable_target TARGET_NAME)
    if(NOT ENABLE_TESTS)
        return()
    endif()

    narc_auto_glob_tests()
    link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})
    add_executable(Tests_${TARGET_NAME} tests/test_main.cpp ${TESTS_CPP_FILES} ${TESTS_HPP_FILES} ${CPP_FILES} ${HPP_FILES})
    add_dependencies(Tests_${TARGET_NAME} ${TARGET_NAME})

    target_link_libraries(Tests_${TARGET_NAME} PRIVATE
        gtest gtest_main
        gmock gmock_main
    )

    gtest_discover_tests(Tests_${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endfunction()

function(narc_testable_add_dependencies TARGET_NAME TARGET_DEPENDENCIES)
    add_dependencies(${TARGET_NAME} ${TARGET_DEPENDENCIES})

    if(ENABLE_TESTS)
        add_dependencies(Tests_${TARGET_NAME} ${TARGET_DEPENDENCIES})
    endif()
endfunction()

function(narc_testable_add_compile_definition TARGET_NAME PROTECTION COMPILE_DEFINITION)
    target_compile_definitions(${TARGET_NAME} ${PROTECTION} ${COMPILE_DEFINITION})

    if(ENABLE_TESTS)
        target_compile_definitions(Tests_${TARGET_NAME} ${PROTECTION} ${COMPILE_DEFINITION})
    endif()
endfunction()

function(narc_testable_add_include_directory TARGET_NAME PROTECTION DIRECTORIES)
    target_include_directories(${TARGET_NAME} ${PROTECTION} ${DIRECTORIES})

    if(ENABLE_TESTS)
        target_include_directories(Tests_${TARGET_NAME} ${PROTECTION} ${DIRECTORIES})
    endif()
endfunction()

function(narc_testable_link_library TARGET_NAME PROTECTION LIBRARIES)
    target_link_libraries(${TARGET_NAME} ${PROTECTION} ${LIBRARIES})

    if(ENABLE_TESTS)
        target_link_libraries(Tests_${TARGET_NAME} ${PROTECTION} ${LIBRARIES})
    endif()
endfunction()

function(narc_testable_pch TARGET_NAME PROTECTION FILES)
    target_precompile_headers(${TARGET_NAME} ${PROTECTION} ${FILES})

    if(ENABLE_TESTS)
        target_precompile_headers(Tests_${TARGET_NAME} ${PROTECTION} ${FILES})
    endif()
endfunction()