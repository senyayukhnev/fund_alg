cmake_minimum_required(VERSION 3.10)

# Название проекта
project(AllocatorComparison)

# Установка стандартов C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Поддиректории для библиотек и исполняемого файла
add_subdirectory(freelist_allocator)
add_subdirectory(buddy_allocator)

# Главный исполняемый файл
add_executable(main_allocator_loader main.c)

# Линковка библиотеки с исполняемым файлом
if (WIN32)
    target_link_libraries(main_allocator_loader freelist_allocator buddy_allocator)
else()
    target_link_libraries(main_allocator_loader dl)
endif()
