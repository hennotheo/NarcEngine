file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_executable(NarcEngineApp src/main.cpp ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngineApp
        CompileShaderCommand
        NarcEngine)

target_include_directories(NarcEngineApp PRIVATE
        ${CMAKE_SOURCE_DIR}/application/include)

target_link_libraries(NarcEngineApp PRIVATE NarcEngine)