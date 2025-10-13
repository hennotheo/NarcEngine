narc_blob_source_files()

find_package(tinyobjloader REQUIRED)
find_package(stb REQUIRED)

add_library(NarcIO SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcIO)

narc_testable_add_dependencies(NarcIO
        NarcLog
        NarcMath)

narc_testable_add_compile_definition(NarcIO PRIVATE NARC_IO_BUILD_DLL)

narc_testable_pch(NarcIO PRIVATE "include/pch.h")

narc_testable_add_include_directory(NarcIO PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcIO PUBLIC NarcLog NarcMath)
narc_testable_link_library(NarcIO PRIVATE stb::stb tinyobjloader::tinyobjloader)