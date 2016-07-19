cmake_minimum_required(VERSION 3.5)
set(EXECUTABLE_NAME sdl2_cube)
project(sdl2_cube)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
		utils.h
        Framelimit.h
		Cube.cpp Cube.h
        Chunk.cpp Chunk.h
        World.cpp World.h
        main.cpp
        Frustum.cpp Frustum.h)

add_executable(sdl2_cube ${SOURCE_FILES})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_TTF REQUIRED)

#include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
endif ()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories($(GLEW_INCLUDE_DIRS))
    target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
endif ()

include_directories(deps/FastNoise)
add_library(FastNoise STATIC deps/FastNoise/FastNoise.cpp deps/FastNoise/FastNoise.h)
target_link_libraries(${EXECUTABLE_NAME} FastNoise)

include_directories(deps/octree)
