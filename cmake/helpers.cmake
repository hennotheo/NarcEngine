function(narc_blob_source_files)
    file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

    set(CPP_FILES ${CPP_FILES} PARENT_SCOPE)
    set(HPP_FILES ${HPP_FILES} PARENT_SCOPE)
endfunction()

function(narc_auto_setup_tests TARGET_NAME)
    add_executable(Tests_${TARGET_NAME} tests/test_main.cpp)

    target_link_libraries(Tests_${TARGET_NAME} PRIVATE
        gtest gtest_main
        gmock gmock_main
        ${TARGET_NAME}
    )
    gtest_discover_tests(Tests_${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endfunction()