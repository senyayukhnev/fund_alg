cmake_minimum_required(VERSION 3.28)
project(OC_lab C)

set(CMAKE_C_STANDARD 11)

add_executable(OC_lab example/posix_ipc-example-client.c)
add_executable(posix_ipc-example-server.c example/posix_ipc-example-server.c)
add_executable(server_lab lab1/server_lab.c)
add_executable(filter_str lab1/filter_str.c)
