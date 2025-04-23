function(narc_blob_source_files)
    file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

    set(CPP_FILES ${CPP_FILES} PARENT_SCOPE)
    set(HPP_FILES ${HPP_FILES} PARENT_SCOPE)
endfunction()