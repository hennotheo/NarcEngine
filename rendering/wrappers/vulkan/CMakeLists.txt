narc_blob_source_files()

add_library(NarcVulkanWrapper STATIC include/NarcVulkanWrapper.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcVulkanWrapper)

narc_testable_add_include_directory(NarcVulkanWrapper PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_precompile_headers(NarcVulkanWrapper PRIVATE "include/pch.h")

narc_testable_link_library(NarcVulkanWrapper PUBLIC
        RhiCore
        NarcEngineDependencies)
