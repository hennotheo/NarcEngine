include(FetchContent)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors/glm
)

FetchContent_MakeAvailable(glm)

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(${NARC_MATH} SHARED include/NarcMath.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(${NARC_MATH} glm)

target_compile_definitions(${NARC_MATH} PRIVATE NARC_MATH_BUILD_DLL)

target_precompile_headers(${NARC_MATH} PRIVATE "include/pch.h")

target_include_directories(${NARC_MATH} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${glm_SOURCE_DIR}")

target_link_libraries(${NARC_MATH} PRIVATE glm)