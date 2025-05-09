set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors)
set(FETCHCONTENT_QUIET ON)

set(CMAKE_POLICY_WARNING_CMP0000 OFF)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

include(FetchContent)
message(STATUS "Fetching dependencies...")

# --- GLM ---

set(GLM_VERSION 1.0.1)

find_package(glm ${GLM_VERSION} QUIET)
if(glm_FOUND)
    message(STATUS "GLM ${GLM_VERSION} found on system.")
    add_library(glm_lib INTERFACE IMPORTED GLOBAL)
    target_link_libraries(glm INTERFACE glm::glm)
else()
    message(STATUS "GLM ${GLM_VERSION} not found, fetching from repository...")
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG ${GLM_VERSION}
        SOURCE_DIR ${VENDOR_DIR}/glm
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(glm)

    add_library(glm_lib INTERFACE IMPORTED GLOBAL)
    target_include_directories(glm_lib INTERFACE ${VENDOR_DIR}/glm)

endif()

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    SOURCE_DIR ${VENDOR_DIR}/stb
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG v2.0.0rc13
    SOURCE_DIR ${VENDOR_DIR}/tinyobjloader
    EXCLUDE_FROM_ALL

)
FetchContent_MakeAvailable(tinyobjloader)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    SOURCE_DIR ${VENDOR_DIR}/glfw
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glfw)

# --- VULKAN ---


find_package(Vulkan REQUIRED)
if(WIN32)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR" CACHE STRING "Vulkan Windows platform flag")

elseif(UNIX)

    message(FATAL_ERROR "Unix platform is not supported yet.")

elseif(APPLE)

    message(FATAL_ERROR "Apple platform is not supported yet.")

endif(WIN32)

if(Vulkan_FOUND)
    message(STATUS "Vulkan found on system.")

    set(VULKAN_INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS})
    set(VULKAN_LIBRARIES ${Vulkan_LIBRARIES})
    set(VULKAN_FOUND ${Vulkan_FOUND})
else()
    message(FATAL_ERROR "Vulkan not found. Please install Vulkan SDK and set VULKAN_SDK environment variable.")
endif()

set(VULKAN_GLSLC_PATH "${VULKAN_PATH}/Bin/glslc.exe" CACHE INTERNAL "Vulkan GLSL Compiler Path")

#--- IMGUI ---
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9
    SOURCE_DIR ${VENDOR_DIR}/imgui
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)

set(imgui_SOURCE_FILES 
    ${VENDOR_DIR}/imgui/imgui.cpp
    ${VENDOR_DIR}/imgui/imgui_draw.cpp
    ${VENDOR_DIR}/imgui/imgui_widgets.cpp
    ${VENDOR_DIR}/imgui/imgui_tables.cpp
    ${VENDOR_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${VENDOR_DIR}/imgui/backends/imgui_impl_vulkan.cpp
)

# --- PYTHON ---
find_package (Python COMPONENTS Interpreter)