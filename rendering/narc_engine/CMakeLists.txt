#----------------
#----- MAIN -----
#----------------

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

set(NARC_ENGINE_DEPENDENCIES
    NarcIO
    NarcLog
    NarcMath
    CompileShaderCommand)

add_library(NarcEngine SHARED include/NarcEngine.h ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngine ${NARC_ENGINE_DEPENDENCIES})

target_compile_definitions(NarcEngine PRIVATE NARC_ENGINE_BUILD_DLL)

target_include_directories(NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(NarcEngine PRIVATE
    "${Vulkan_INCLUDE_DIRS}")

target_link_libraries(NarcEngine PUBLIC
    NarcIO
    NarcLog
    NarcMath)
target_link_libraries(NarcEngine PRIVATE
    glfw
    Vulkan::Vulkan)

target_precompile_headers(NarcEngine PRIVATE "include/pch.h")

#----------------
#----- TEST -----
#----------------

# add_executable(NarcEngine_Tests tests/test_main.cpp ${CPP_FILES} ${HPP_FILES})

# add_dependencies(NarcEngine_Tests
#         googleTest
#         ${NARC_ENGINE_DEPENDENCIES})

# target_link_directories(NarcEngine_Tests PRIVATE
#         ${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE}
#         ${CMAKE_CURRENT_SOURCE_DIR}/vendors/lib
#         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
#         ${VULKAN_PATH}/Lib
#         "${VENDORS_DIR}/lib")

# target_compile_definitions(NarcEngine_Tests PRIVATE NARC_ENGINE_BUILD_DLL)

# target_include_directories(NarcEngine_Tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
# target_include_directories(NarcEngine_Tests PRIVATE
#         "${VULKAN_PATH}/Include"
#         "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include"
#         "${VENDORS_DIR}/include")

# target_link_libraries(NarcEngine_Tests PRIVATE
#         NarcIO
#         NarcLog
#         NarcMath
#         gtest gtest_main
#         gmock gmock_main
#         ${GLFW_LIB_LIST}
#         ${VULKAN_LIB_LIST})

# target_precompile_headers(NarcEngine_Tests PRIVATE "include/pch.h")