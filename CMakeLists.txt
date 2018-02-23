cmake_minimum_required(VERSION 3.9)
project(miniSnake)

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

add_executable(miniSnake
        snake.c snake_opengl.c snake.h)
