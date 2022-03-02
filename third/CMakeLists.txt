
# Cereal

set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
set(JUST_INSTALL_CEREAL ON CACHE BOOL "" FORCE)
set(WITH_WERROR OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(cereal)

# EnTT

add_subdirectory(entt)

# GLFW

if(DEFINED ENV{GITHUB_OS})
    find_package(GLFW)
endif()

if(NOT GLFW_FOUND)
    message(STATUS "GLFW not found. Compiling our own version.")
    # We could download glfw at this point, instead of using a submodule.

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
    else()
        message(STATUS "GLFW found. using found version.")
endif()

# GLM

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(glm)

# BGFX

set(BGFX_BUILD_TOOLS ON CACHE BOOL "" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(bgfx)

# ImGUI

project(imgui)
file(GLOB IMGUI_SOURCES
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_demo.cpp

#  imgui/backends/imgui_impl_glfw.cpp
#  imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${IMGUI_SOURCES})
target_link_libraries(imgui PRIVATE glfw)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)

# ImGuizmo

project(imguizmo)
file(GLOB IMGUIZMO_SOURCES
  imguizmo/GraphEditor.cpp
  imguizmo/ImCurveEdit.cpp
  imguizmo/ImGradient.cpp
  imguizmo/ImGuizmo.cpp
  imguizmo/ImSequencer.cpp
)
add_library(imguizmo ${IMGUIZMO_SOURCES})
target_link_libraries(imguizmo PRIVATE imgui)
target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo)

# ImTerm

project(imterm)
add_library(imterm INTERFACE)

target_include_directories(imterm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ImTerm/include)

# PhysX

if(DEFINED ENV{PHYSX_PATH})
    set(PHYSX_ROOT_DIR ${ENV{PHYSX_PATH}})
    message(STATUS "Using PhysX path: '${PHYSX_ROOT_DIR}'")
else()
    set(PHYSX_ROOT_DIR "${CMAKE_SOURCE_DIR}/third/physx/physx")
    message(STATUS "Using default PhysX path: '${PHYSX_ROOT_DIR}'")
endif()
set(ENV{PHYSX_PATH} ${PHYSX_ROOT_DIR})

set(PHYSX_INCLUDE_DIRS $ENV{PHYSX_PATH}/include/ $ENV{PHYSX_PATH}/../pxshared/include/ PARENT_SCOPE)
set(PHYSX_LIBRARIES
    PhysXExtensions
    PhysX
    PhysXPvdSDK
    PhysXVehicle
    PhysXCharacterKinematic
    PhysXCooking
    PhysXCommon
    PhysXFoundation
PARENT_SCOPE)

# has to match the TARGET_BUILD_PLATFORM in $ENV{PHYSX_PATH}/physix/buildtools/cmake_generate_projects.py
if(WIN32)
    set(TARGET_BUILD_PLATFORM "windows")
elseif(LINUX)
    set(TARGET_BUILD_PLATFORM "linux")
endif()

set(PX_BUILDSNIPPETS OFF CACHE BOOL "Generate the snippets")
set(PX_BUILDPUBLICSAMPLES OFF CACHE BOOL "Generate the samples projects")
set(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "Generate static libraries")
set(PX_FLOAT_POINT_PRECISE_MATH OFF CACHE BOOL "Float point precise math")
set(NV_USE_STATIC_WINCRT OFF CACHE BOOL "Use the statically linked windows CRT")
set(NV_USE_DEBUG_WINCRT ON CACHE BOOL "Use the debug version of the CRT")
set(PXSHARED_PATH $ENV{PHYSX_PATH}/../pxshared)
set(PXSHARED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CMAKEMODULES_VERSION "1.27")
set(CMAKEMODULES_PATH $ENV{PHYSX_PATH}/../externals/cmakemodules)

set(PX_OUTPUT_LIB_DIR ${CMAKE_BINARY_DIR}/third/physx)
set(PX_OUTPUT_BIN_DIR ${CMAKE_BINARY_DIR}/third/physx)

add_subdirectory($ENV{PHYSX_PATH}/compiler/public)

# portable-file-dialogs

add_subdirectory(portable-file-dialogs)

# RapidJSON

project(rapidjson)
add_library(rapidjson INTERFACE)

target_include_directories(rapidjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)

# SPDLog

set(SPDLOG_MASTER_PROJECT OFF CACHE BOOL "" FORCE)
add_subdirectory(spdlog)

# STB

project(stb)
add_library(stb INTERFACE)

target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# stduuid

set(UUID_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(stduuid)

# Yojimbo

set(sodium_USE_STATIC_LIBS SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
find_package(sodium REQUIRED)
set(MBEDTLS_USE_STATIC_LIBS SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
find_package(MbedTLS REQUIRED)

project(Yojimbo C CXX)
add_library(yojimbo STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo/yojimbo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo/tlsf/tlsf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo/netcode.io/netcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo/reliable.io/reliable.c
)

target_include_directories(yojimbo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo/netcode.io
    ${CMAKE_CURRENT_SOURCE_DIR}/yojimbo/reliable.io
)

target_link_libraries(yojimbo PRIVATE sodium)

target_include_directories(yojimbo PUBLIC ${MBEDTLS_INCLUDE_DIR})
