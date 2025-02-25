#----------------
#----- MAIN -----
#----------------

set(NARC_ENGINE_LIB_LIST "NarcEngine")

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(NarcEngineApp src/main.cpp ${CPP_FILES} ${HPP_FILES})

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_dependencies(NarcEngineApp ${NARC_ENGINE})
include_directories(${CMAKE_SOURCE_DIR}/application/include)
include_directories(${NARC_ENGINE_INCLUDE})

target_link_libraries(NarcEngineApp PRIVATE ${NARC_ENGINE_LIB_LIST})