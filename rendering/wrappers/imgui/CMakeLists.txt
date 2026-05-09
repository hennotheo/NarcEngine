narc_blob_source_files()

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

add_library(NarcImguiWrapper STATIC include/NarcImguiWrapper.h ${CPP_FILES} ${HPP_FILES})
narc_decl_testable_target(NarcImguiWrapper)

narc_testable_add_include_directory(NarcImguiWrapper PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include")

narc_testable_add_include_directory(NarcImguiWrapper PRIVATE
        "${imgui_INCLUDE_DIRS}/../res/bindings")

target_precompile_headers(NarcImguiWrapper PRIVATE "include/pch.h")

get_filename_component(IMGUI_PACKAGE_ROOT "${imgui_INCLUDE_DIRS}" DIRECTORY)

set(IMGUI_BINDINGS_DIR "${IMGUI_PACKAGE_ROOT}/res/bindings")

message(STATUS "IMGUI_PACKAGE_ROOT = ${IMGUI_PACKAGE_ROOT}")
message(STATUS "IMGUI_BINDINGS_DIR = ${IMGUI_BINDINGS_DIR}")

target_sources(NarcImguiWrapper PRIVATE
        "${IMGUI_BINDINGS_DIR}/imgui_impl_glfw.cpp"
        "${IMGUI_BINDINGS_DIR}/imgui_impl_vulkan.cpp")

narc_testable_link_library(NarcImguiWrapper PUBLIC
        RhiCore
        NarcEngineDependencies
        imgui::imgui
        NarcVulkanWrapper
        glfw)
