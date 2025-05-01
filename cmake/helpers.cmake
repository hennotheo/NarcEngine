function(narc_blob_source_files)
    file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

    set(CPP_FILES ${CPP_FILES} PARENT_SCOPE)
    set(HPP_FILES ${HPP_FILES} PARENT_SCOPE)
endfunction()

function(narc_set_shared_lib)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared-libgcc" PARENT_SCOPE)
    endif()
endfunction()