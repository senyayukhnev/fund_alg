# CMakeLists.txt для библиотеки buddy_allocator

# Установка минимальной версии CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(buddy_allocator)

# Добавление исходников библиотеки
add_library(buddy_allocator SHARED
        buddy_allocator.c
)
