cmake_minimum_required(VERSION 3.18)
project(lec2)

set(CMAKE_C_STANDARD 99)

add_executable(main src/main.c)
target_link_libraries(main ncursesw)
