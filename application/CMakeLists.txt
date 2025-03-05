file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_executable(${NARC_ENGINE_APP} src/main.cpp ${CPP_FILES} ${HPP_FILES})

add_dependencies(${NARC_ENGINE_APP}
        ${COMPILE_SHADER_COMMAND}
        ${NARC_ENGINE})

target_include_directories(${NARC_ENGINE_APP} PRIVATE
        ${CMAKE_SOURCE_DIR}/application/include)

target_link_libraries(${NARC_ENGINE_APP} PRIVATE ${NARC_ENGINE})

install(TARGETS
        ${NARC_ENGINE_APP})