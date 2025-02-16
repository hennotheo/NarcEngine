message(STATUS "Building NarcEngine")

#----------------------
#----- GET VULKAN -----
#----------------------

option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

if(AUTO_LOCATE_VULKAN)
    message(STATUS "Attempting auto locate Vulkan using CMake......")

    # Find Vulkan Path using CMake's Vulkan Module
    # This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
    # Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
    find_package(Vulkan)

    # Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
    if(NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
        set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
        STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
    endif()

    if(NOT Vulkan_FOUND)
        # CMake may fail to locate the libraries but could be able to 
        # provide some path in Vulkan SDK include directory variable
        # 'Vulkan_INCLUDE_DIRS', try to extract path from this.
        message(STATUS "Failed to locate Vulkan SDK, retrying again...")
        if(EXISTS "${VULKAN_PATH}")
            message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
        else()
            message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
            message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
            return()
        endif()
    endif()
else()
    message(STATUS "Attempting to locate Vulkan SDK using manual path......")
    set(VULKAN_SDK "C:/VulkanSDK")
    set(VULKAN_VERSION "1.0.33.0")
    set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
    message(STATUS "Using manual specified path: ${VULKAN_PATH}")

    # Check if manual set path exists
    if(NOT EXISTS "${VULKAN_PATH}")
        message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.
		For more information on correct installation process, please refer to subsection 'Getting started with Lunar-G SDK'
		and 'Setting up first project with CMake' in Chapter 3, 'Shaking hands with the device' in this book 'Learning Vulkan', ISBN - 9781786469809.")
        return()
    endif()
endif()

#----------------
#----- MAIN -----
#----------------

set(NARC_ENGINE "NarcEngine")
set(VULKAN_LIB_LIST "vulkan-1")
set(GLFW_LIB_LIST "glfw3")

variable_watch(GLFW_LIB_LIST)

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories("vendors/glfw/lib-vc2022")
link_directories(${VULKAN_PATH}/Lib)

add_compile_definitions(NARC_ENGINE_BUILD_DLL)

add_library(${NARC_ENGINE} SHARED include/NarcEngine.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(AFTER ${VULKAN_PATH}/Include)
include_directories("vendors/glm")
include_directories("vendors/glfw/include")

target_link_libraries(${NARC_ENGINE} PRIVATE ${VULKAN_LIB_LIST})
target_link_libraries(${NARC_ENGINE} PRIVATE ${GLFW_LIB_LIST})

target_precompile_headers(${NARC_ENGINE} PRIVATE "include/pch.h")

add_custom_command(TARGET ${NARC_ENGINE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NARC_ENGINE}> ${SANDBOX_BIN_DIR}/$<TARGET_FILE_NAME:${NARC_ENGINE}>
)

#--------------------
#--- Shader FILES ---
#--------------------

set(VULKAN_GLSLC_PATH ${VULKAN_PATH}/Bin/glslc.exe)
set(SHADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(GLOB VERTEX_SHADER_FILES "shaders/*.vert")
file(GLOB FRAGMENT_SHADER_FILES "shaders/*.frag")

set(SHADER_COMMANDS "")

foreach(VERTEX_SHADER_FILE ${VERTEX_SHADER_FILES})
    get_filename_component(VERTEX_SHADER_FILENAME ${VERTEX_SHADER_FILE} NAME)
    string(REGEX REPLACE ".vert" "" VERTEX_SHADER_FILENAME_NO_EXTENSION ${VERTEX_SHADER_FILENAME})
    list(APPEND SHADER_COMMANDS "\n${VULKAN_GLSLC_PATH} ${VERTEX_SHADER_FILENAME_NO_EXTENSION}.vert -o ${VERTEX_SHADER_FILENAME_NO_EXTENSION}_vert.spv")
endforeach()

foreach(FRAGMENT_SHADER_FILE ${FRAGMENT_SHADER_FILES})
    get_filename_component(FRAGMENT_SHADER_FILENAME ${FRAGMENT_SHADER_FILE} NAME)
    string(REGEX REPLACE ".frag" "" FRAGMENT_SHADER_FILENAME_NO_EXTENSION ${FRAGMENT_SHADER_FILENAME})
    list(APPEND SHADER_COMMANDS "\n${VULKAN_GLSLC_PATH} ${FRAGMENT_SHADER_FILENAME_NO_EXTENSION}.frag -o ${FRAGMENT_SHADER_FILENAME_NO_EXTENSION}_frag.spv")
endforeach()

file(WRITE ${SHADER_PATH}/compile_shaders.bat ${SHADER_COMMANDS})

add_custom_target(
    CompileShaders ALL
    WORKING_DIRECTORY ${SHADER_PATH}
    COMMENT "Compiling Shaders..."
    COMMAND ${SHADER_PATH}/compile_shaders.bat
)

file(GLOB SHADER_FILES "shaders/*.spv")
file(COPY ${SHADER_FILES} DESTINATION ${SANDBOX_BIN_DIR}/shaders/)
