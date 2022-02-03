file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/res)

# Textures

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/res/textures)

file (GLOB_RECURSE KNOTING_TEXTURES LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/res/textures/*")
add_custom_target(knoting_textures ALL DEPENDS ${KNOTING_TEXTURES})

foreach(TEXTURE_FILE ${KNOTING_TEXTURES})
    get_filename_component(FILE_NAME ${TEXTURE_FILE} NAME)
    get_filename_component(PARENT_DIR ${TEXTURE_FILE} DIRECTORY)
    string(REGEX REPLACE "^${CMAKE_SOURCE_DIR}/res/textures" "" PARENT_DIR ${PARENT_DIR})

    set(FILE_NAME "${CMAKE_BINARY_DIR}/dist/res/textures/${PARENT_DIR}/${FILE_NAME}")
    configure_file(${TEXTURE_FILE} ${FILE_NAME} COPYONLY)
endforeach()

add_dependencies(tie knoting_textures)
add_dependencies(untie knoting_textures)

# MISC

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/res/misc)

file (GLOB_RECURSE KNOTING_RES LIST_DIRECTORIES false "${CMAKE_SOURCE_DIR}/res/misc/*")
add_custom_target(knoting_res_misc ALL DEPENDS ${KNOTING_RES})

foreach(RES_FILE ${KNOTING_RES})
    get_filename_component(FILE_NAME ${RES_FILE} NAME)
    get_filename_component(PARENT_DIR ${RES_FILE} DIRECTORY)
    string(REGEX REPLACE "^${CMAKE_SOURCE_DIR}/res/misc" "" PARENT_DIR ${PARENT_DIR})

    set(FILE_NAME "${CMAKE_BINARY_DIR}/dist/res/misc/${PARENT_DIR}/${FILE_NAME}")
    configure_file(${RES_FILE} ${FILE_NAME} COPYONLY)
endforeach()

add_dependencies(tie knoting_res_misc)
add_dependencies(untie knoting_res_misc)
