narc_blob_source_files()

set(NARC_ENGINE_DEPENDENCIES
    NarcIO
    NarcLog
    NarcMath
    CompileShaderCommand)

set(PRIVATE_LIBS
    glfw
    Vulkan::Vulkan)

add_library(NarcEngine SHARED include/NarcEngine.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngine ${NARC_ENGINE_DEPENDENCIES})

target_compile_definitions(NarcEngine PRIVATE NARC_ENGINE_BUILD_DLL)

target_include_directories(NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(NarcEngine PRIVATE "${Vulkan_INCLUDE_DIRS}")

target_link_libraries(NarcEngine PUBLIC
    NarcIO
    NarcLog
    NarcMath)
target_link_libraries(NarcEngine PRIVATE ${PRIVATE_LIBS})
target_precompile_headers(NarcEngine PRIVATE "include/pch.h")


#----------------
#----- TEST -----
#----------------

if(ENABLE_TESTS)
    narc_auto_glob_tests()
    link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE} )
    add_executable(Tests_NarcEngine tests/test_main.cpp ${TESTS_CPP_FILES} ${TESTS_HPP_FILES} ${CPP_FILES} ${HPP_FILES})

    add_dependencies(Tests_NarcEngine NarcEngine ${NARC_ENGINE_DEPENDENCIES})

    target_include_directories(Tests_NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(Tests_NarcEngine PRIVATE "${Vulkan_INCLUDE_DIRS}")

    target_compile_definitions(Tests_NarcEngine PRIVATE TESTING_BUILD_DLL)

    target_link_libraries(Tests_NarcEngine PRIVATE
        gtest gtest_main
        gmock gmock_main
        ${PRIVATE_LIBS}
        NarcIO
        NarcLog
        NarcMath)

    target_precompile_headers(Tests_NarcEngine PRIVATE "include/pch.h")
    gtest_discover_tests(Tests_NarcEngine
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)   
endif()
