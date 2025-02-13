# CMakeLists.txt для библиотеки freelist_allocator

# Установка минимальной версии CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(freelist_allocator)

# Добавление исходников библиотеки
add_library(freelist_allocator SHARED
        freelist_allocator.c
)

# Установка каталога для создания библиотеки (опционально)
set_target_properties(freelist_allocator PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
