cmake_minimum_required(VERSION 3.28)
project(fund_list)

set(CMAKE_CXX_STANDARD 20)

add_executable(fund_list
        include/container.h
        include/list.h
        tests/tests.cpp)
