cmake_minimum_required(VERSION 3.31)

include(cmake/versionning.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release")
endif ()

project(NarcEngineEditor VERSION ${NARC_VERSION})

include(cmake/helpers.cmake)
include(cmake/dependencies.cmake)
include(cmake/tests.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_BUILD_DEBUG" APPEND)
endif()

set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS "NARC_ENGINE_PLATFORM_WINDOWS" APPEND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

add_subdirectory(core/core)
add_subdirectory(core/narc_math)
add_subdirectory(core/narclog)
add_subdirectory(core/narcio)

add_subdirectory(rendering/shaders)
add_subdirectory(rendering/narc_engine)

add_subdirectory(application)

# # Install Path
# install(TARGETS
#         NarcEngineEditor
#         NarcEngine
#         NarcMath
#         NarcLog
#         NarcIO
#         RUNTIME DESTINATION /)
# install(DIRECTORY
#         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
#         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures
#         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/models
#         DESTINATION /)

# if (MINGW)
#     find_program(MINGW_BIN_DIR NAMES mingw32-gcc gcc PATHS ENV PATH NO_DEFAULT_PATH)
#     if (NOT MINGW_BIN_DIR)
#         message(SEND_ERROR "MinGW installation not found. Please ensure MinGW is installed and available in the PATH.")
#     else ()
#         get_filename_component(MINGW_BIN_DIR "${MINGW_BIN_DIR}" DIRECTORY)
#     endif ()

#     set_target_properties(NarcEngineEditor PROPERTIES ENABLE_EXPORTS ON)
#     install(CODE "
#         file(GLOB MINGW_DLLS
#             \"${MINGW_BIN_DIR}/libgcc_s_seh-1.dll\"
#             \"${MINGW_BIN_DIR}/libstdc++-6.dll\"
#             \"${MINGW_BIN_DIR}/libwinpthread-1.dll\"
#         )
#         message(\"COPYING DLLS: \${MINGW_DLLS}\")
#         foreach(DLL \${MINGW_DLLS})
#             file(INSTALL \${DLL} DESTINATION \${CMAKE_INSTALL_PREFIX}/)
#         endforeach()
#     ")
# endif ()

# set(CMAKE_INSTALL_MESSAGE LAZY)

# #Config Cpack
# include(InstallRequiredSystemLibraries)
# set(CPACK_PACKAGE_NAME "NarcEngine")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "NarcEngine")
# set(CPACK_PACKAGE_VERSION_MAJOR "${NARC_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${NARC_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${NARC_VERSION_PATCH}")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
# include(CPack)
