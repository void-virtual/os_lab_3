cmake_minimum_required(VERSION 3.14)
project(OS_lab_3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(OS_lab_3 main.c adjacency_list.c queue.c)
add_executable(test test.c adjacency_list.c queue.c)
