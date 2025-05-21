narc_blob_source_files()

add_library(NarcRHI STATIC include/Rhi.h ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(NarcRHI PRIVATE NARC_ENGINE_BUILD_DLL)

target_include_directories(NarcRHI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(NarcRHI PRIVATE ${PRIVATE_INCLUDES})

target_link_libraries(NarcRHI PUBLIC
    NarcIO
    NarcLog
    NarcMath)
target_link_libraries(NarcRHI PRIVATE NarcEngineDependencies)
# target_precompile_headers(NarcRHI priv "include/pch.h")