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