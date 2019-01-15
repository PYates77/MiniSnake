cmake_minimum_required(VERSION 3.5)
project(miniSnake)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../ /usr/include)

add_executable(snake_ascii ${CMAKE_CURRENT_SOURCE_DIR}/snake_ascii.c)

target_link_libraries(snake_ascii /usr/lib/x86_64-linux-gnu/libcurses.so)

install(TARGETS snake_ascii RUNTIME DESTINATION /usr/local/bin)

