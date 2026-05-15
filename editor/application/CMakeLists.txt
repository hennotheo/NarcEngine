file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


add_executable(NarcEngineEditor src/main.cpp ${CPP_FILES} ${HPP_FILES})
add_dependencies(NarcEngineEditor NarcRHI)

target_include_directories(NarcEngineEditor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(NarcEngineEditor PRIVATE NarcRHI NarcLog NarcVulkanWrapper NarcCore NarcMath NarcIO NarcGraphicsEngine)
target_precompile_headers(NarcEngineEditor PRIVATE "include/pch.h")
