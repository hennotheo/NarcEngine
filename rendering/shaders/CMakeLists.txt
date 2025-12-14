find_program(Vulkan_GLSLC_EXECUTABLE NAMES glslc)

add_custom_target(CompileShaderCommand
        COMMENT "Compiling shaders"
        COMMAND ${CMAKE_COMMAND} -E env GLSLC_BIN="${Vulkan_GLSLC_EXECUTABLE}" ENGINE_BIN_DIR="${NarcEngineEditor_BINARY_DIR}/shaders/" ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compile_shaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)