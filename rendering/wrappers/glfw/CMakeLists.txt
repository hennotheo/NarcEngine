narc_blob_source_files()

find_package(glfw3 REQUIRED)

add_library(NarcGlfwWrapper STATIC include/NarcGlfwWrapper.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcGlfwWrapper)

narc_testable_add_include_directory(NarcGlfwWrapper PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_precompile_headers(NarcGlfwWrapper PRIVATE "include/pch.h")

narc_testable_link_library(NarcGlfwWrapper PUBLIC
        RhiCore
        NarcEngineDependencies
        glfw)
