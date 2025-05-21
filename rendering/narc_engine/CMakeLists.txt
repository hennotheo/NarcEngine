narc_blob_source_files()

add_library(NarcEngine SHARED include/NarcEngine.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngine ${NARC_ENGINE_DEPENDENCIES} NarcRHI)

target_compile_definitions(NarcEngine PRIVATE NARC_ENGINE_BUILD_DLL)

target_include_directories(NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_sources(NarcEngine PRIVATE ${imgui_SOURCE_FILES})

target_link_libraries(NarcEngine PUBLIC
    NarcIO
    NarcLog
    NarcMath
    NarcRHI)
target_link_libraries(NarcEngine PRIVATE NarcEngineDependencies)
target_precompile_headers(NarcEngine PRIVATE "include/pch.h")

#----------------
#----- TEST -----
#----------------

if(ENABLE_TESTS)
    narc_auto_glob_tests()
    link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})
    add_executable(Tests_NarcEngine tests/test_main.cpp ${TESTS_CPP_FILES} ${TESTS_HPP_FILES} ${CPP_FILES} ${HPP_FILES})

    add_dependencies(Tests_NarcEngine NarcEngine ${NARC_ENGINE_DEPENDENCIES})

    target_include_directories(Tests_NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(Tests_NarcEngine PRIVATE ${PRIVATE_INCLUDES} "${CMAKE_CURRENT_SOURCE_DIR}/tests")

    target_compile_definitions(Tests_NarcEngine PRIVATE NARC_ENGINE_BUILD_DLL)

    target_link_libraries(Tests_NarcEngine PRIVATE
        gtest gtest_main
        gmock gmock_main
        NarcIO
        NarcLog
        NarcMath)

    target_precompile_headers(Tests_NarcEngine PRIVATE "include/pch.h")
    gtest_discover_tests(Tests_NarcEngine
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
