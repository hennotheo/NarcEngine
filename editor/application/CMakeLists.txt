file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_executable(NarcEngineEditor src/main.cpp ${CPP_FILES} ${HPP_FILES})
add_dependencies(NarcEngineEditor NarcRHI NarcLog)

target_include_directories(NarcEngineEditor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(NarcEngineEditor PRIVATE NarcRHI NarcLog)
target_precompile_headers(NarcEngineEditor PRIVATE "include/pch.h")