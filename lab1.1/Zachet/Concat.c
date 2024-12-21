#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// Функция для подсчета длины строки
int my_strlen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

// Функция для копирования строки
void my_strcpy(char* dest, const char* src) {
    int i = 0;
    while ((dest[i] = src[i]) != '\0') {
        i++;
    }
}

// Функция для конкатенации строки
void my_strcat(char* dest, const char* src) {
    int dest_len = my_strlen(dest);
    int i = 0;
    while ((dest[dest_len + i] = src[i]) != '\0') {
        i++;
    }
}

// Функция для конкатенации переменного количества строк
// result - указатель на указатель, куда будет записан результат
// first_str - первая строка для конкатенации
// ... - остальные строки для конкатенации
void concat(char** result, char* first_str, ...) {
    va_list args; // Список аргументов
    int total_length = my_strlen(first_str); // Общая длина всех строк, начинаем с первой строки
    char* current_str;

    // Инициализация списка аргументов
    va_start(args, first_str);

    // Подсчет общей длины всех строк
    while ((current_str = va_arg(args, char*)) != NULL) {
        total_length += my_strlen(current_str); // Добавляем длину каждой строки
    }

    // Завершение работы со списком аргументов
    va_end(args);

    // Выделение памяти для результирующей строки
    *result = (char*)malloc(total_length + 1); // +1 для завершающего нулевого символа
    if (*result == NULL) {
        printf("Ошибка выделения памяти\n");
        return;
    }

    // Копирование первой строки в результат
    my_strcpy(*result, first_str);

    // Конкатенация остальных строк
    va_start(args, first_str); // Снова инициализируем список аргументов
    while ((current_str = va_arg(args, char*)) != NULL) {
        my_strcat(*result, current_str); // Добавляем очередную строку
    }
    va_end(args); // Завершение работы со списком аргументов
}

int main() {
    char* result = NULL; // Указатель на результирующую строку

    // Конкатенация строк
    concat(&result, "Hello, ", "world", "!", NULL);

    // Вывод результата
    if (result != NULL) {
        printf("Результат: %s\n", result);
        free(result); // Освобождаем выделенную память
    }

    return 0;
}