#include <stdio.h>
#include <stdarg.h>

// Функция для поиска подстроки в переменном количестве строк с флагом завершения (NULL)
int find_substring_in_strings(const char* substr, int* count, ...) {
    va_list args; // Список аргументов
    const char* current_str;
    int total_occurrences = 0; // Общее количество вхождений подстроки
    int substr_len = 0;

    // Вычисляем длину подстроки
    while (substr[substr_len] != '\0') substr_len++;

    // Инициализация списка аргументов
    va_start(args, count);

    // Итерируемся по строкам, пока не встретим NULL
    while ((current_str = va_arg(args, const char*)) != NULL) {
        int str_len = 0;
        while (current_str[str_len] != '\0') str_len++; // Вычисляем длину текущей строки

        // Поиск подстроки в текущей строке
        for (int i = 0; i <= str_len - substr_len; i++) {
            int j;
            for (j = 0; j < substr_len; j++) {
                if (current_str[i + j] != substr[j]) {
                    break; // Символы не совпадают
                }
            }

            // Если все символы совпали, подстрока найдена
            if (j == substr_len) {
                total_occurrences++;
            }
        }
    }

    // Завершение работы со списком аргументов
    va_end(args);

    *count = total_occurrences; // Возвращаем общее количество вхождений
    return (total_occurrences > 0) ? 1 : 0; // Возвращаем 1, если подстрока найдена хотя бы раз, иначе 0
}