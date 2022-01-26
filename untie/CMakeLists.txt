file(GLOB_RECURSE UNTIE_SOURCES
    **/*.cpp
    **/*.h
)

add_executable(untie ${UNTIE_SOURCES})

set_target_properties(untie PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(untie PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
set_target_properties(untie PROPERTIES OUTPUT_NAME untie)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(untie PUBLIC KNOTING_DEBUG=1)
endif()

target_link_libraries(untie PRIVATE knoting)

configure_debugging(untie WORKING_DIR ${CMAKE_BINARY_DIR}/dist)
