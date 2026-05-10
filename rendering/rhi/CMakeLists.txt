narc_blob_source_files()

add_library(NarcRHI STATIC public/Rhi.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcRHI)

narc_testable_add_compile_definition(NarcRHI PRIVATE NARC_ENGINE_BUILD_DLL)

narc_testable_add_include_directory(NarcRHI PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/public")

narc_testable_add_include_directory(NarcRHI PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcRHI PUBLIC
        RhiCore)

narc_testable_link_library(NarcRHI PRIVATE
        NarcVulkanWrapper
        NarcGlfwWrapper
        NarcImguiWrapper)
