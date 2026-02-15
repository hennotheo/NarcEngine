set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCES_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(folder_list
        "models"
        "textures"
)

foreach (folder ${folder_list})
    file(GLOB_RECURSE RESOURCE_FILES "${RESOURCES_PATH}/${folder}/*")
    file(COPY ${RESOURCE_FILES} DESTINATION ${RESOURCES_OUTPUT_PATH}/${folder})
endforeach ()