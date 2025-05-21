add_subdirectory(shaders)

add_library(NarcI INTERFACE)
target_include_directories(NarcI INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_precompile_headers(NarcI INTERFACE "rhi/include/pch.h")

set(NARC_ENGINE_DEPENDENCIES
    NarcIO
    NarcLog
    NarcMath
    CompileShaderCommand)

set(PRIVATE_LIBS
    glfw
    Vulkan::Vulkan
    NarcI)

set(PRIVATE_INCLUDES
    "${Vulkan_INCLUDE_DIRS}"
    "${imgui_SOURCE_DIR}")

add_subdirectory(rhi)
add_subdirectory(narc_engine)