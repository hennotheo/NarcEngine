add_custom_target(CompileShaderCommand
        COMMENT "Compiling shaders"
        COMMAND ${CMAKE_COMMAND} -E env ENGINE_BIN_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/" ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compile_shaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)