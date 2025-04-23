narc_blob_source_files()

add_library(NarcCore STATIC include/NarcCore.h ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(NarcCore PRIVATE NARC_CORE_BUILD_DLL)

target_include_directories(NarcCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_precompile_headers(NarcCore PUBLIC "include/pch.h")