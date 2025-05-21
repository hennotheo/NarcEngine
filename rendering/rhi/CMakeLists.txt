narc_blob_source_files()

add_library(NarcRHI STATIC include/Rhi.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcRHI)

narc_testable_add_compile_definition(NarcRHI PRIVATE NARC_ENGINE_BUILD_DLL)

narc_testable_add_include_directory(NarcRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcRHI PUBLIC 
    NarcIO
    NarcLog
    NarcMath)
narc_testable_link_library(NarcRHI PRIVATE NarcEngineDependencies)
# target_precompile_headers(NarcRHI priv "include/pch.h")

# if(ENABLE_TESTS)
#     narc_auto_setup_tests(NarcRHI)
#     target_compile_definitions(Tests_NarcRHI PRIVATE NARC_ENGINE_BUILD_DLL)
#     target_include_directories(Tests_NarcRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
#     target_link_libraries(Tests_NarcRHI PRIVATE 
#         NarcEngineDependencies
#         NarcIO
#         NarcLog
#         NarcMath)
# endif()