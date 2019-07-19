cmake_minimum_required(VERSION 3.5)
project(miniSnake)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ /usr/include)

list(APPEND CMAKE_PREFIX_PATH "E:/Tools/glew-2.1.0")
list(APPEND CMAKE_PREFIX_PATH "E:/Tools/freeglut")

# Find packages of additional libraries
#   - GLEW
find_package( GLEW REQUIRED )
if ( GLEW_FOUND )
    include_directories( ${GLEW_INCLUDE_DIRS} )
    link_libraries     ( ${GLEW_LIBRARIES}    )
endif()

#   - GLUT
find_package( GLUT REQUIRED )
if ( GLUT_FOUND )
    include_directories( ${GLUT_INCLUDE_DIR} )
    link_libraries     ( ${GLUT_LIBRARIES}    )
endif()

#   - OpenGL
find_package( OpenGL REQUIRED )
if ( OPENGL_FOUND )
    include_directories( ${OPENGL_INCLUDE_DIRS} )
    link_libraries     ( ${OPENGL_LIBRARIES}    )
endif()

set(CMAKE_CXX_STANDARD 11)

add_executable(miniSnake snake_opengl.c highScore.c)

# uncomment or modify this line for the ai snake games
add_executable(planningSnake planning_ai.c)
add_executable(mazeSnake maze_ai.c)
