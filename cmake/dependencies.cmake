set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors)
set(FETCHCONTENT_QUIET ON)

set(CMAKE_POLICY_WARNING_CMP0000 OFF)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    SOURCE_DIR ${VENDOR_DIR}/glm
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glm)

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
find_package(Vulkan)

if(WIN32)

    if(NOT Vulkan_FOUND)
        find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
        if(Vulkan_LIBRARY)
            set(Vulkan_FOUND ON)
            message("Using bundled Vulkan library version")
        endif()
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")

elseif(UNIX)

    message(FATAL_ERROR "Apple platform is not supported yet.")

elseif(APPLE)

    message(FATAL_ERROR "Apple platform is not supported yet.")

endif(WIN32)

if(Vulkan_FOUND)
    set(VULKAN_INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS})
    set(VULKAN_LIBRARIES ${Vulkan_LIBRARIES})
    set(VULKAN_FOUND ${Vulkan_FOUND})
else()
    message(FATAL_ERROR "Vulkan not found. Please install Vulkan SDK and set VULKAN_SDK environment variable.")
endif()

set(VULKAN_GLSLC_PATH "${VULKAN_PATH}/Bin/glslc.exe" CACHE INTERNAL "Vulkan GLSL Compiler Path")

# --- PYTHON ---
find_package (Python COMPONENTS Interpreter)