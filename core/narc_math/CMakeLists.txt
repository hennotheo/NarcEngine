narc_blob_source_files()

add_library(NarcMath SHARED include/NarcMath.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcMath glm)

target_compile_definitions(NarcMath PRIVATE NARC_MATH_BUILD_DLL)

target_precompile_headers(NarcMath PRIVATE "include/pch.h")

target_include_directories(NarcMath PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${glm_SOURCE_DIR}")

target_link_libraries(NarcMath PRIVATE glm)