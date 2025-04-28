if(ENABLE_TESTS)
    return()
endif()
    

set(CMAKE_INSTALL_MESSAGE LAZY)

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

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "NarcEngine")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "NarcEngine")
set(CPACK_PACKAGE_VERSION_MAJOR "${NARC_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${NARC_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${NARC_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)
