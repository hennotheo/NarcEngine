narc_blob_source_files()

add_library(NarcLog SHARED include/NarcLog.h ${CPP_FILES} ${HPP_FILES})

target_compile_definitions(NarcLog PRIVATE NARCLOG_BUILD_DLL)
target_precompile_headers(NarcLog PRIVATE "include/pch.h")

add_dependencies(NarcLog NarcCore)

target_include_directories(NarcLog PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(NarcLog PUBLIC NarcCore)

#----------------
#----- TEST -----
#----------------

narc_auto_setup_tests(NarcLog "")