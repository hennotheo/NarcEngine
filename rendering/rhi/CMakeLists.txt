narc_blob_source_files()

add_library(NarcRHI STATIC include/rhi/Rhi.h ${CPP_FILES} ${HPP_FILES}
        tests/swapchain/test_swapchain_base.h)
narc_decl_testable_target(NarcRHI)

narc_testable_add_compile_definition(NarcRHI PRIVATE NARC_ENGINE_BUILD_DLL)

narc_testable_add_include_directory(NarcRHI PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include/rhi"
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcRHI PUBLIC 
    NarcIO
    NarcLog
    NarcMath)
narc_testable_link_library(NarcRHI PRIVATE NarcEngineDependencies)