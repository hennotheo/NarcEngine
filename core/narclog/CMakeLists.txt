narc_blob_source_files()

add_library(NarcLog SHARED include/NarcLog.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcLog)

narc_testable_add_dependencies(NarcLog NarcCore)

narc_testable_add_compile_definition(NarcLog PRIVATE NARCLOG_BUILD_DLL)
narc_testable_pch(NarcLog PRIVATE "include/pch.h")


narc_testable_add_include_directory(NarcLog PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcLog PUBLIC NarcCore)