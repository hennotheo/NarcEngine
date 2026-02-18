narc_blob_source_files()

add_library(RhiCore STATIC include/RhiCore.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(RhiCore)

narc_testable_add_include_directory(RhiCore PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(RhiCore PUBLIC
        NarcIO
        NarcLog
        NarcMath)

target_precompile_headers(RhiCore PUBLIC "include/pch.h")