cmake_minimum_required(VERSION 3.28.3)
project(lab1_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(laba1.c
        Lab1/laba1.c
)
add_executable(laba2 Lab1/laba2.c)
add_executable(laba6.c Lab1/laba6.c)
add_executable(laba5.c Lab1/laba5.c)
add_executable(laba3.c Lab1/laba3.c)
add_executable(another6 to_test_smth.c
        test/main.c)
add_executable(laba4 Lab1/lab4/laba4.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=200809L")

add_executable(laba7 Lab1/lab7/laba7.c)
add_executable(laba8 Lab1/lab8/laba8.c)
add_executable(laba9 Lab1/laba9.c)
add_executable(laba10 Lab1/laba10.c)
add_executable(laba2_7 Lab2/laba2_7.c)
add_executable(laba2_2 Lab2/laba2_2.c)
add_executable(laba2_1 Lab2/laba2_1.c)
add_executable(laba2_10 Lab2/laba2_10.c)
add_executable(laba2_4 Lab2/laba2_4.c)
add_executable(laba3 Lab2/laba2_3.c)
add_executable(laba2_8 Lab2/laba2_8.c)
add_executable(laba3_3 Lab3/lab3/laba3_3.c)
add_executable(laba3_1 Lab3/laba3_1.c)
add_executable(laba3_10 Lab3/lab10/laba3_10.c)
add_executable(laba3_2 Lab3/laba3_2.c)

target_link_libraries(laba3_3 m)