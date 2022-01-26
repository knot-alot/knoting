file(GLOB_RECURSE KNOTING_EDITOR_SOURCES
    **/*.cpp
    **/*.h
)

add_executable(knoting_editor ${KNOTING_EDITOR_SOURCES})

set_target_properties(knoting_editor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(knoting_editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
set_target_properties(knoting_editor PROPERTIES OUTPUT_NAME tie)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(knoting_editor PUBLIC KNOTING_DEBUG=1)
endif()

target_link_libraries(knoting_editor PRIVATE knoting)
