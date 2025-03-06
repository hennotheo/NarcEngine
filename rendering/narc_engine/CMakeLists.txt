#----------------------
#----- GET VULKAN -----
#----------------------

option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

if (AUTO_LOCATE_VULKAN)
    message(STATUS "Attempting auto locate Vulkan using CMake......")

    # Find Vulkan Path using CMake's Vulkan Module
    # This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
    # Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
    find_package(Vulkan)

    # Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
    if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
        set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
        STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
    endif ()

    if (NOT Vulkan_FOUND)
        # CMake may fail to locate the libraries but could be able to 
        # provide some path in Vulkan SDK include directory variable
        # 'Vulkan_INCLUDE_DIRS', try to extract path from this.
        message(STATUS "Failed to locate Vulkan SDK, retrying again...")
        if (EXISTS "${VULKAN_PATH}")
            message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
        else ()
            message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
            message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
            return()
        endif ()
    endif ()
else ()
    message(STATUS "Attempting to locate Vulkan SDK using manual path......")
    set(VULKAN_SDK "C:/VulkanSDK")
    set(VULKAN_VERSION "1.0.33.0")
    set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
    message(STATUS "Using manual specified path: ${VULKAN_PATH}")

    # Check if manual set path exists
    if (NOT EXISTS "${VULKAN_PATH}")
        message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.
		For more information on correct installation process, please refer to subsection 'Getting started with Lunar-G SDK'
		and 'Setting up first project with CMake' in Chapter 3, 'Shaking hands with the device' in this book 'Learning Vulkan', ISBN - 9781786469809.")
        return()
    endif ()
endif ()

set(VULKAN_GLSLC_PATH "${VULKAN_PATH}/Bin/glslc.exe" CACHE INTERNAL "Vulkan GLSL Compiler Path")

#------------------------
#----- Dependencies -----
#------------------------

include(ExternalProject)
ExternalProject_Add(
        glfw
        GIT_REPOSITORY
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

#----------------
#----- MAIN -----
#----------------

set(VULKAN_LIB_LIST "vulkan-1")
set(GLFW_LIB_LIST "glfw3")

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendors/lib)
link_directories(${VULKAN_PATH}/Lib)

add_library(NarcEngine SHARED include/NarcEngine.h ${CPP_FILES} ${HPP_FILES}) # ${CPP_FILES} ${HPP_FILES})

add_dependencies(NarcEngine
        NarcIO
        NarcLog
        NarcMath
        glfw)

target_compile_definitions(NarcEngine PRIVATE NARC_ENGINE_BUILD_DLL)

target_include_directories(NarcEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(NarcEngine PRIVATE
        "${VULKAN_PATH}/Include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendors/include")

target_link_libraries(NarcEngine PUBLIC
        NarcIO
        NarcLog
        NarcMath)
target_link_libraries(NarcEngine PRIVATE
        ${GLFW_LIB_LIST}
        ${VULKAN_LIB_LIST})

target_precompile_headers(NarcEngine PRIVATE "include/pch.h")