narc_blob_source_files()

add_library(NarcCore STATIC include/NarcCore.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcCore)

narc_testable_add_compile_definition(NarcCore PRIVATE NARC_CORE_BUILD_DLL)

narc_testable_add_include_directory(NarcCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_pch(NarcCore PUBLIC "include/pch.h")