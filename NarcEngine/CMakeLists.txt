message(STATUS "Building NarcEngine")

#----------------------
#----- GET VULKAN -----
#----------------------

option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

if(AUTO_LOCATE_VULKAN)
	message(STATUS "Attempting auto locate Vulkan using CMake......")
	
	# Find Vulkan Path using CMake's Vulkan Module
	# This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
	# Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
	find_package(Vulkan)
	
	# Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
	if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
	endif()
		 
	if(NOT Vulkan_FOUND)
		# CMake may fail to locate the libraries but could be able to 
		# provide some path in Vulkan SDK include directory variable
		# 'Vulkan_INCLUDE_DIRS', try to extract path from this.
		message(STATUS "Failed to locate Vulkan SDK, retrying again...")
		if(EXISTS "${VULKAN_PATH}")
			message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
		else()
			message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
			message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
			return()
		endif()
	endif()
else()
	message(STATUS "Attempting to locate Vulkan SDK using manual path......")
	set(VULKAN_SDK "C:/VulkanSDK")
	set(VULKAN_VERSION "1.0.33.0")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	message(STATUS "Using manual specified path: ${VULKAN_PATH}")

	# Check if manual set path exists
	if(NOT EXISTS "${VULKAN_PATH}")
		message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.
		For more information on correct installation process, please refer to subsection 'Getting started with Lunar-G SDK'
		and 'Setting up first project with CMake' in Chapter 3, 'Shaking hands with the device' in this book 'Learning Vulkan', ISBN - 9781786469809.")
	   return()
	endif()
endif()

#----------------
#--- CMD FILES---
#----------------

set(VULKAN_GLSLC_PATH ${VULKAN_PATH}/Bin/glslc.exe)
set(SHADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(WRITE ${SHADER_PATH}/compile-shaders.bat
	${VULKAN_GLSLC_PATH} " shader.vert -o vert.spv \n"
	${VULKAN_GLSLC_PATH} " shader.frag -o frag.spv \n"
	pause
)


#----------------
#----- MAIN -----
#----------------

set(NARC_ENGINE "NarcEngine")
set(VULKAN_LIB_LIST "vulkan-1")
set(GLFW_LIB_LIST "glfw3")

# Gather list of header and source files for compilation
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

link_directories("vendors/glfw/lib-vc2022")
link_directories(${VULKAN_PATH}/Lib)

add_library(${NARC_ENGINE} SHARED main.cpp)# ${CPP_FILES} ${HPP_FILES})

# # Define project properties
# set(NARCENGINE_BIN_PATH ${PROJECT_SOURCE_DIR}/binaries/x64)
# set(NARCENGINE_BIN_INT_PATH ${PROJECT_SOURCE_DIR}/binaries/int)

# set_property(TARGET ${NARC_ENGINE} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${NARCENGINE_BIN_PATH})
# set_property(TARGET ${NARC_ENGINE} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${NARCENGINE_BIN_PATH}/Debug)
# set_property(TARGET ${NARC_ENGINE} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${NARCENGINE_BIN_PATH}/Release)
# set_property(TARGET ${NARC_ENGINE} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${NARCENGINE_BIN_PATH}/MinSizeRel)
# set_property(TARGET ${NARC_ENGINE} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${NARCENGINE_BIN_PATH}/RelWithDebInfo)

# set_property(TARGET ${NARC_ENGINE} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${NARCENGINE_BIN_INT_PATH})
# set_property(TARGET ${NARC_ENGINE} PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG ${NARCENGINE_BIN_INT_PATH}/Debug)
# set_property(TARGET ${NARC_ENGINE} PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE ${NARCENGINE_BIN_INT_PATH}/Release)
# set_property(TARGET ${NARC_ENGINE} PROPERTY LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${NARCENGINE_BIN_INT_PATH}/MinSizeRel)
# set_property(TARGET ${NARC_ENGINE} PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${NARCENGINE_BIN_INT_PATH}/RelWithDebInfo)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(AFTER ${VULKAN_PATH}/Include)
include_directories("vendors/glm")
include_directories("vendors/glfw/include")


target_link_libraries( ${NARC_ENGINE} PRIVATE ${VULKAN_LIB_LIST} )
target_link_libraries( ${NARC_ENGINE} PRIVATE ${GLFW_LIB_LIST} )

#-----------------------------
#----- POST BUILD MOVING -----
#-----------------------------

file(GLOB SHADER_FILES "shaders/*.spv")
file(COPY ${SHADER_FILES} DESTINATION ${SANDBOX_BIN_DIR}/shaders/)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${CONFIGURATION}/${NARC_ENGINE}.dll DESTINATION ${SANDBOX_BIN_DIR}/)