narc_blob_source_files()

add_library(NarcEngine SHARED include/NarcEngine.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcEngine)

narc_testable_add_dependencies(NarcEngine ${NARC_ENGINE_DEPENDENCIES} NarcRHI)

narc_testable_add_compile_definition(NarcEngine PRIVATE NARC_ENGINE_BUILD_DLL)

narc_testable_add_include_directory(NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_target_sources(NarcEngine PRIVATE ${imgui_SOURCE_FILES})

narc_testable_link_library(NarcEngine PUBLIC
    NarcIO
    NarcLog
    NarcMath
    NarcRHI)
narc_testable_link_library(NarcEngine PRIVATE NarcEngineDependencies)

narc_testable_pch(NarcEngine PRIVATE "include/pch.h")