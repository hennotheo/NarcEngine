add_subdirectory(shaders)

#-----------------
#--- INTERFACE ---
#-----------------

add_library(NarcEngineDependencies INTERFACE)
target_include_directories(NarcEngineDependencies INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${Vulkan_INCLUDE_DIRS}"
        "${imgui_SOURCE_DIR}")
target_link_libraries(NarcEngineDependencies INTERFACE
        glfw
        Vulkan::Vulkan
        di_lib)
target_precompile_headers(NarcEngineDependencies INTERFACE "rhi/include/pch.h")

add_dependencies(NarcEngineDependencies
        NarcIO
        NarcLog
        NarcMath
        CompileShaderCommand)

#-----------------
#----- SOURCE -----
#-----------------

add_subdirectory(rhi)
add_subdirectory(narc_engine)