file(GLOB_RECURSE TIE_SOURCES
    **/*.cpp
    **/*.h
)

add_executable(tie ${TIE_SOURCES})

set_target_properties(tie PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(tie PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
set_target_properties(tie PROPERTIES OUTPUT_NAME tie)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(tie PUBLIC KNOTING_DEBUG=1)
endif()

target_link_libraries(tie PRIVATE knoting)
target_link_libraries(tie PRIVATE portable_file_dialogs)
target_link_libraries(tie PRIVATE imterm)

configure_debugging(tie WORKING_DIR ${CMAKE_BINARY_DIR}/dist) # BGFX
