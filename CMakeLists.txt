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

set(PROCESSOR_ARCH "x64")

# Dependencies
# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# GLAD
file(TO_CMAKE_PATH "$ENV{GLAD_SRC}/src/glad.c" GLAD_SRC_C)
set(GLAD_SRC $ENV{GLAD_SRC})
set(GLAD_INCLUDE "${GLAD_SRC}/include")

# GLM
set(GLM_SRC "$ENV{GLM_SRC}/glm")
set(GLM_INCLUDE "${GLM_SRC}")

# ASSIMP
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
add_subdirectory($ENV{ASSIMP_SRC} "$ENV{ASSIMP_SRC}/build")
set(ASSIMP_SRC $ENV{ASSIMP_SRC})
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
message(STATUS "  GLAD_SRC:     ${GLAD_SRC}")
message(STATUS "  GLM_SRC:      ${GLM_SRC}")
message(STATUS "  ASSIMP_SRC:   ${ASSIMP_SRC}")
message(STATUS "")
message(STATUS "Dependencies include:")
message(STATUS "  SDL:           ${SDL2_INCLUDE_DIRS}")
message(STATUS "  GLAD:          ${GLAD_INCLUDE}")
message(STATUS "  GLM:           ${GLM_INCLUDE}")
message(STATUS "  ASSIMP:        ${ASSIMP_INCLUDE}")
message(STATUS "  ASSIMP_BUILD:  ${ASSIMP_BUILD_INCLUDE}")
message(STATUS "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add -g for valgrind
add_compile_options(-g)

file(GLOB learn-opengl_source
  "${learn-opengl_source_dir}/[a-z0-9_-]*.c*"
  "${GLAD_SRC_C}"
)

add_executable(learn-opengl ${learn-opengl_source})

if(MSVC)
  target_link_libraries(learn-opengl assimp)
elseif(UNIX)
  target_link_libraries(learn-opengl ${SDL2_LIBRARIES} GL X11 pthread Xrandr Xi dl assimp)
  
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

target_include_directories(
  learn-opengl PUBLIC
  ${learn-opengl_include_dir}
  ${GLAD_INCLUDE}
  ${GLM_INCLUDE}
  ${ASSIMP_INCLUDE}
  ${ASSIMP_BUILD_INCLUDE}
)
