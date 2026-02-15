narc_blob_source_files()

add_library(NarcRenderingCore STATIC include/NarcRenderingCore.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcRenderingCore)

narc_testable_add_include_directory(NarcRenderingCore PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcRenderingCore PUBLIC
        NarcIO
        NarcLog
        NarcMath)

target_precompile_headers(NarcRenderingCore PUBLIC "include/pch.h")