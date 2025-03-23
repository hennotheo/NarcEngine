cmake_minimum_required(VERSION 3.10)

include(ExternalProject)
ExternalProject_Add(
        googleTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.x
        PREFIX ./vendors
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_GMOCK=OFF
)

set(VENDORS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors CACHE STRING "Vendors directory")

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

set(ENGINE_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" CACHE STRING "Binary path")
set(ENGINE_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" CACHE STRING "Library path")
set(VULKAN_GLSLC_PATH "" CACHE INTERNAL "Vulkan GLSL Compiler Path")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Choose the type of build" FORCE)

set(NARC_VERSION_MAJOR "0")
set(NARC_VERSION_MINOR "1")
set(NARC_VERSION_PATCH "0")
set(NARC_VERSION "${NARC_VERSION_MAJOR}.${NARC_VERSION_MINOR}.${NARC_VERSION_PATCH}")

project(NarcEngineApp VERSION ${NARC_VERSION})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_BUILD_DEBUG" APPEND)
endif ()
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_ENGINE_PLATFORM_WINDOWS" APPEND)

add_subdirectory(core/core)
add_subdirectory(core/narc_math)
add_subdirectory(core/narclog)
add_subdirectory(core/narcio)

add_subdirectory(rendering/shaders)
add_subdirectory(rendering/narc_engine)

add_subdirectory(application)

# Install Path
install(TARGETS
        NarcEngineEditor
        NarcEngine
        NarcMath
        NarcLog
        NarcIO
        RUNTIME DESTINATION /)
install(DIRECTORY
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/models
        DESTINATION /)

if (MINGW)
    find_program(MINGW_BIN_DIR NAMES mingw32-gcc gcc PATHS ENV PATH NO_DEFAULT_PATH)
    if (NOT MINGW_BIN_DIR)
        message(SEND_ERROR "MinGW installation not found. Please ensure MinGW is installed and available in the PATH.")
    else ()
        get_filename_component(MINGW_BIN_DIR "${MINGW_BIN_DIR}" DIRECTORY)
    endif ()

    set_target_properties(NarcEngineEditor PROPERTIES ENABLE_EXPORTS ON)
    install(CODE "
        file(GLOB MINGW_DLLS
            \"${MINGW_BIN_DIR}/libgcc_s_seh-1.dll\"
            \"${MINGW_BIN_DIR}/libstdc++-6.dll\"
            \"${MINGW_BIN_DIR}/libwinpthread-1.dll\"
        )
        message(\"COPYING DLLS: \${MINGW_DLLS}\")
        foreach(DLL \${MINGW_DLLS})
            file(INSTALL \${DLL} DESTINATION \${CMAKE_INSTALL_PREFIX}/)
        endforeach()
    ")
endif ()

set(CMAKE_INSTALL_MESSAGE LAZY)

#Config Cpack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "NarcEngine")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "NarcEngine")
set(CPACK_PACKAGE_VERSION_MAJOR "${NARC_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${NARC_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${NARC_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)