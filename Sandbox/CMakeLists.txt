message(STATUS "Building Sandbox")

#----------------
#----- MAIN -----
#----------------

set(NARC_ENGINE_LIB_LIST "NarcEngine")

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_executable(${SANDBOX} src/TestEngine.cpp)

include_directories(${CMAKE_SOURCE_DIR}/core/narc_engine/include/)

target_link_libraries(${SANDBOX} PRIVATE ${NARC_ENGINE_LIB_LIST})

set_target_properties(${SANDBOX}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)