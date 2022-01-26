cmake_minimum_required(VERSION 3.2)

project(knoting CXX C)

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument -Wunused-command-line-argument")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -Wunused-command-line-argument")

    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fstandalone-debug")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fstandalone-debug")

    if(EXISTS "/usr/bin/mold")
        add_link_options("-fuse-ld=/usr/bin/mold")
    endif()
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/dist/res)

add_subdirectory(third)
add_subdirectory(core)
add_subdirectory(editor)
add_subdirectory(untie)
