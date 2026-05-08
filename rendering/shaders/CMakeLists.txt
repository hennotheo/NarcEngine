find_program(Vulkan_GLSLC_EXECUTABLE NAMES glslc)

if (NOT Vulkan_GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Run Conan install first so shaderc is available in the preset toolchain environment.")
endif ()

add_custom_target(CompileShaderCommand
        COMMENT "Compiling shaders"
        COMMAND ${CMAKE_COMMAND} -E env GLSLC_BIN="${Vulkan_GLSLC_EXECUTABLE}" ENGINE_BIN_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/" ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compile_shaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
