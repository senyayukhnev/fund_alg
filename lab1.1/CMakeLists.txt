cmake_minimum_required(VERSION 3.28.3)
project(lab1_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(laba1.c
        laba1.c
)
add_executable(laba2 laba2.c)
add_executable(laba6.c laba6.c)
add_executable(laba5.c laba5.c)
add_executable(laba3.c laba3.c)
add_executable(another6 laba6_2.c)
add_executable(laba4 laba4.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=200809L")

