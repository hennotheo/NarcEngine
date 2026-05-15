narc_blob_source_files()

add_library(NarcGraphicsEngine STATIC public/NarcGraphicsEngine.h ${CPP_FILES} ${HPP_FILES})

narc_testable_add_compile_definition(NarcGraphicsEngine PRIVATE NARC_ENGINE_BUILD_DLL)

narc_testable_add_include_directory(NarcGraphicsEngine PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/public")

narc_testable_add_include_directory(NarcGraphicsEngine PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcGraphicsEngine PRIVATE
        NarcRHI)

target_precompile_headers(NarcGraphicsEngine PUBLIC "public/pch.h")