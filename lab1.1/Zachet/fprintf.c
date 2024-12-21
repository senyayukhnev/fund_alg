#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// Упрощённая реализация fprintf
void my_fprintf(FILE* stream, const char* format, ...) {
    va_list args; // Список аргументов
    va_start(args, format); // Инициализация списка аргументов

    while (*format != '\0') {
        if (*format == '%') { // Если встретился спецификатор формата
            format++; // Переходим к символу после '%'

            // Обработка спецификаторов формата
            switch (*format) {
                case 'd': { // Целое число
                    int value = va_arg(args, int);
                    char buffer[32]; // Буфер для преобразования числа в строку
                    int i = 0;

                    // Преобразуем число в строку
                    if (value < 0) {
                        fputc('-', stream); // Записываем знак минуса
                        value = -value;
                    }
                    do {
                        buffer[i++] = (value % 10) + '0'; // Добавляем цифру
                        value /= 10;
                    } while (value > 0);

                    // Записываем строку в обратном порядке
                    while (i > 0) {
                        fputc(buffer[--i], stream);
                    }
                    break;
                }
                case 'f': { // Число с плавающей точкой
                    double value = va_arg(args, double);
                    char buffer[128]; // Буфер для преобразования числа в строку
                    int i = 0;

                    // Преобразуем число в строку
                    if (value < 0) {
                        fputc('-', stream); // Записываем знак минуса
                        value = -value;
                    }

                    // Преобразуем целую часть
                    long long int_part = (long long)value;
                    double frac_part = value - int_part;

                    // Преобразуем целую часть
                    if (int_part == 0) {
                        fputc('0', stream);
                    } else {
                        char int_buffer[32];
                        int j = 0;
                        while (int_part > 0) {
                            int_buffer[j++] = (int_part % 10) + '0';
                            int_part /= 10;
                        }
                        while (j > 0) {
                            fputc(int_buffer[--j], stream);
                        }
                    }

                    // Преобразуем дробную часть
                    fputc('.', stream);
                    for (int j = 0; j < 6; j++) { // Ограничиваем точность до 6 знаков
                        frac_part *= 10;
                        int digit = (int)frac_part;
                        fputc(digit + '0', stream);
                        frac_part -= digit;
                    }
                    break;
                }
                case 's': { // Строка
                    char* value = va_arg(args, char*);
                    while (*value != '\0') {
                        fputc(*value++, stream); // Записываем символы строки
                    }
                    break;
                }
                case 'c': { // Символ
                    int value = va_arg(args, int); // char автоматически приводится к int
                    fputc(value, stream); // Записываем символ
                    break;
                }
                default: { // Неизвестный спецификатор
                    fputc('%', stream); // Выводим '%'
                    fputc(*format, stream); // Выводим спецификатор как есть
                    break;
                }
            }
        } else { // Обычный символ
            fputc(*format, stream); // Выводим символ в поток
        }
        format++; // Переходим к следующему символу
    }

    va_end(args); // Завершение работы со списком аргументов
}