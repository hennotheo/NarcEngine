narc_blob_source_files()

find_package(glm REQUIRED)
if(NOT glm_FOUND)
    message(FATAL_ERROR "GLM not found on system.")
endif()

add_library(NarcMath SHARED include/NarcMath.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcMath)

add_dependencies(NarcMath NarcCore)

narc_testable_add_compile_definition(NarcMath PRIVATE NARC_MATH_BUILD_DLL)

narc_testable_pch(NarcMath PRIVATE "include/pch.h")

narc_testable_add_include_directory(NarcMath PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")


narc_testable_link_library(NarcMath PUBLIC NarcCore glm::glm)