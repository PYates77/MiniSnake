cmake_minimum_required(VERSION 3.5)
project(miniSnake)

set(CMAKE_CXX_STANDARD 11)

include_directories(include /usr/include)

add_executable(asciiSnake ${CMAKE_CURRENT_SOURCE_DIR}/src/snake.c ${CMAKE_CURRENT_SOURCE_DIR}/src/snake_ascii.c)

target_link_libraries(asciiSnake /usr/lib/x86_64-linux-gnu/libcurses.so)

