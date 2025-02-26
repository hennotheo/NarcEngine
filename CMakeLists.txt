cmake_minimum_required(VERSION 3.10)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexceptions")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

set(ENGINE_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" CACHE INTERNAL "Binary path")
set(ENGINE_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" CACHE INTERNAL "Library path")
set(VULKAN_GLSLC_PATH "" CACHE INTERNAL "Vulkan GLSL Compiler Path")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

project(NarcEngine VERSION 0.1)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_BUILD_DEBUG")
endif ()
set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_ENGINE_PLATFORM_WINDOWS" APPEND)

add_subdirectory(core/narclog)
add_subdirectory(core/narcio)
add_subdirectory(core/narc_engine)

add_subdirectory(application)

add_subdirectory(core/shaders)

# Install Path
install(TARGETS
        ${NARC_ENGINE_APP}
        ${NARC_IO}
        ${NARC_ENGINE}
        DESTINATION /)
install(DIRECTORY
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures
        DESTINATION /)

if (MINGW)
    if (NOT DEFINED ENV{MINGW_HOME})
        message(SEND_ERROR "MINGW_HOME environment variable is not set. Please set it to the MinGW installation directory.")
    endif ()

    set(MINGW_BIN_DIR "$ENV{MINGW_HOME}/bin")

    set_target_properties(NarcEngineApp PROPERTIES ENABLE_EXPORTS ON)
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

#Config Cpack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "NarcEngine")
set(CPACK_PACKAGE_VERSION_MAJOR "${NarcEngine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${NarcEngine_VERSION_MINOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)