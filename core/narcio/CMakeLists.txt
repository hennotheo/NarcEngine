narc_blob_source_files()

add_library(NarcIO SHARED include/NarcIO.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcIO
        NarcLog
        NarcMath)

target_compile_definitions(NarcIO PRIVATE
        NARC_IO_BUILD_DLL)
target_precompile_headers(NarcIO PRIVATE
        "include/pch.h")

target_include_directories(NarcIO PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(NarcIO PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
        "${stb_SOURCE_DIR}"
        "${tinyobjloader_SOURCE_DIR}"
)

target_link_libraries(NarcIO PUBLIC NarcLog NarcMath)

narc_auto_setup_tests(NarcIO)