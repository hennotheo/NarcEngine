add_custom_target(CompileShaderCommand
        COMMENT "Compiling shaders"
        COMMAND ${CMAKE_COMMAND} -E env ENGINE_BIN_DIR="${ENGINE_BIN_DIR}/shaders/" python3 ${CMAKE_CURRENT_SOURCE_DIR}/compile_shaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)