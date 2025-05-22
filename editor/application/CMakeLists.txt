file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories(${CMAKE_BINARY_DIR}/NarcEngine/${CMAKE_BUILD_TYPE})

add_executable(NarcEngineEditor src/main.cpp ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcEngineEditor)

narc_testable_add_dependencies(NarcEngineEditor
        NarcEngine)

narc_testable_add_include_directory(NarcEngineEditor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_link_library(NarcEngineEditor PRIVATE NarcEngine)
narc_testable_pch(NarcEngineEditor PRIVATE "include/pch.h")