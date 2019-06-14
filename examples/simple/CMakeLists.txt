cmake_minimum_required(VERSION 3.5)
project(miniSnake)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ /usr/include)

add_executable(snake_simple ${CMAKE_CURRENT_SOURCE_DIR}/snake_simple.c)
