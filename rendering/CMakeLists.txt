add_subdirectory(shaders)

#-----------------
#--- INTERFACE ---
#-----------------

find_package(Vulkan REQUIRED)

find_package(VulkanMemoryAllocator REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(imgui REQUIRED)

# TODO: TEMPORARY SOLUTION
add_library(NarcEngineDependencies INTERFACE)
target_include_directories(NarcEngineDependencies INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(NarcEngineDependencies INTERFACE
        Vulkan::Vulkan
        imgui::imgui
        vulkan-headers::vulkan-headers
        GPUOpen::VulkanMemoryAllocator)
#target_precompile_headers(NarcEngineDependencies INTERFACE "rhi/include/pch.h")

add_dependencies(NarcEngineDependencies
        CompileShaderCommand
        NarcIO
        NarcLog
        NarcMath)

#------------------
#----- SOURCE -----
#------------------

add_subdirectory(rhi_core)
add_subdirectory(wrappers)
add_subdirectory(rhi)
add_subdirectory(graphics_engine)

#------------------
#---- Keywords ----
#------------------


if (WIN32)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR" CACHE STRING "Vulkan Windows platform flag")

elseif (UNIX AND NOT APPLE)


    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR" CACHE STRING "Vulkan Linux platform flag (XCB)")
    add_compile_definitions(VK_USE_PLATFORM_XCB_KHR)

elseif (APPLE)

    message(FATAL_ERROR "Apple platform is not supported yet.")

endif ()
