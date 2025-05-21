add_subdirectory(shaders)

set(NARC_ENGINE_DEPENDENCIES
    NarcIO
    NarcLog
    NarcMath
    CompileShaderCommand)

set(PRIVATE_LIBS
    glfw
    Vulkan::Vulkan)

set(PRIVATE_INCLUDES
    "${Vulkan_INCLUDE_DIRS}"
    "${imgui_SOURCE_DIR}")

add_subdirectory(rhi)
add_subdirectory(narc_engine)