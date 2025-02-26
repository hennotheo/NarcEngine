#----------------
#----- MAIN -----
#----------------

set(NARC_ENGINE_APP "NarcEngineApp" CACHE STRING "" FORCE)

set(NARC_ENGINE_LIB_LIST
        ${NARC_ENGINE}
        ${NARC_LOG})

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(${NARC_ENGINE_APP} src/main.cpp ${CPP_FILES} ${HPP_FILES})

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_dependencies(${NARC_ENGINE_APP}
        ${COMPILE_SHADER_COMMAND}
        ${NARC_ENGINE}
        ${NARC_LOG})
include_directories(${CMAKE_SOURCE_DIR}/application/include)
include_directories(
        ${NARC_ENGINE_INCLUDE}
        ${NARC_LOG_INCLUDE})

target_link_libraries(${NARC_ENGINE_APP} PRIVATE ${NARC_ENGINE_LIB_LIST})