cmake_minimum_required(VERSION 3.9)
project(miniSnake)

# Help Windows find libaries
if ( WIN32 )
    # NOTE: You should add these directories to your PATH (for .dlls):
    #           C:\Tools\glew-2.0.0\bin\Release\Win32
    #           C:\Tools\freeglut-3.0.0\bin
    # NOTE: After installing NVidia Cg, FindGLUT was finding the GLUT in NVidia's Cg dir.
    set(CMAKE_PREFIX_PATH "C:/Tools/glew-2.1.0;C:/Tools/freeglut")
    set(CMAKE_LIBRARY_PATH "C:/Tools/glew-2.1.0/lib/Release/x64;C:/Tools/freeglut/lib/x64")
endif()

# Set variable
#SET( EXEC tst )

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
