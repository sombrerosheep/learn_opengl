cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(learn-opengl C CXX)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(learn-opengl_description "learn opengl from learnopengl.com")
set(learn-opengl_source_dir "${CMAKE_SOURCE_DIR}/src")
set(learn-opengl_include_dir "${CMAKE_SOURCE_DIR}/include")

# Check for 64 or 32 bit
set(PROCESSOR_ARCH "x64")

# Dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory($ENV{GLFW_SRC} "$ENV{GLFW_SRC}/build")

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory($ENV{ASSIMP_SRC} "$ENV{ASSIMP_SRC}/build")

file(TO_CMAKE_PATH "$ENV{GLAD_SRC}/src/glad.c" GLAD_SRC_C)

set(GLFW_SRC $ENV{GLFW_SRC})
set(GLAD_SRC $ENV{GLAD_SRC})
set(GLM_SRC $ENV{GLM_SRC})
set(ASSIMP_SRC $ENV{ASSIMP_SRC})

set(GLFW_INCLUDE "${GLFW_SRC}/include")
set(GLAD_INCLUDE "${GLAD_SRC}/include")
set(GLM_INCLUDE "${GLM_SRC}")
set(ASSIMP_INCLUDE "${ASSIMP_SRC}/include")
set(ASSIMP_BUILD_INCLUDE "${ASSIMP_SRC}/build/include")

##### Info output #####
message(STATUS "")
message(STATUS "Building ${CMAKE_PROJECT_NAME} Version ${PROJECT_VERSION}")
message(STATUS "${learn-opengl_description}")
message(STATUS "")
message(STATUS "Platform:   ${CMAKE_SYSTEM}")
message(STATUS "Compiler:   ${CMAKE_CXX_COMPILER}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Project Variables:")
message(STATUS "  Source Dir:           ${learn-opengl_source_dir}")
message(STATUS "  Include Dir:          ${learn-opengl_include_dir}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  GLFW_SRC:     ${GLFW_SRC}")
message(STATUS "  GLAD_SRC:     ${GLAD_SRC}")
message(STATUS "  GLM_SRC:      ${GLM_SRC}")
message(STATUS "  ASSIMP_SRC:   ${ASSIMP_SRC}")
message(STATUS "")
message(STATUS "Dependencies include:")
message(STATUS "  GLFW:          ${GLFW_INCLUDE}")
message(STATUS "  GLAD:          ${GLAD_INCLUDE}")
message(STATUS "  GLM:           ${GLM_INCLUDE}")
message(STATUS "  ASSIMP:        ${ASSIMP_INCLUDE}")
message(STATUS "  ASSIMP_BUILD:  ${ASSIMP_BUILD_INCLUDE}")
message(STATUS "")

#### Build ####
file(GLOB learn-opengl_source
  "${learn-opengl_source_dir}/[a-z0-9_-]*.c*"
  "${GLAD_SRC_C}"
)
add_executable(learn-opengl ${learn-opengl_source})

if(MSVC)
  target_link_libraries(learn-opengl glfw assimp)
elseif(UNIX)
# TODO: ...this
  target_link_libraries(learn-opengl glfw3 GL X11 pthread Xrandr Xi dl)
endif()

target_include_directories(
  learn-opengl PUBLIC
  ${learn-opengl_include_dir}
  ${GLFW_INCLUDE}
  ${GLAD_INCLUDE}
  ${GLM_INCLUDE}
  ${ASSIMP_INCLUDE}
  ${ASSIMP_BUILD_INCLUDE}
)
