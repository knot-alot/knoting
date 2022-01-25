file(GLOB_RECURSE KNOTING_SOURCES
    src/*.cpp
    inc/**/*.h
)

set(PUBLIC_LIBS
    glm
    spdlog
)

set(PRIVATE_LIBS
    glfw
    bgfx
    bx
    bimg
)

add_library(knoting ${KNOTING_SOURCES})

set_target_properties(knoting PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(knoting PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)
set_target_properties(knoting PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)
set_target_properties(knoting PROPERTIES OUTPUT_NAME knoting)

# target_compile_definitions(knoting PUBLIC ENTT_ID_TYPE=std::uint64_t)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(knoting PUBLIC KNOTING_DEBUG=1)
else()
    target_compile_definitions(knoting PUBLIC ENTT_DISABLE_ASSERT=1)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(knoting PUBLIC KNOTING_64=1)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    target_compile_definitions(knoting PUBLIC KNOTING_32=1)
endif()

foreach(LIB ${PUBLIC_LIBS})
    target_link_libraries(knoting PUBLIC ${LIB})
endforeach()

foreach(LIB ${PRIVATE_LIBS})
    target_link_libraries(knoting PRIVATE ${LIB})
endforeach()

target_include_directories(knoting PUBLIC ${CMAKE_SOURCE_DIR}/core/inc)

# RUNTIME

add_executable(knoting_runtime ${KNOTING_SOURCES})

set_target_properties(knoting_runtime PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_target_properties(knoting_runtime PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
set_target_properties(knoting_runtime PROPERTIES OUTPUT_NAME knoting)

foreach(LIB ${PUBLIC_LIBS})
    target_link_libraries(knoting_runtime PUBLIC ${LIB})
endforeach()

foreach(LIB ${PRIVATE_LIBS})
    target_link_libraries(knoting_runtime PRIVATE ${LIB})
endforeach()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(knoting_runtime PUBLIC KNOTING_DEBUG=1)
endif()
target_compile_definitions(knoting_runtime PUBLIC KNOTING_RUNTIME=1)

target_include_directories(knoting_runtime PUBLIC ${CMAKE_SOURCE_DIR}/core/inc)
