#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

// Функция для сравнения строк без учёта регистра
int str_case_cmp(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
        }
        str1++;
        str2++;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

int main() {
    FILE* input_file = fopen("input.txt", "r");
    FILE* output_file = fopen("output.txt", "w");

    if (input_file == NULL) {
        fprintf(stderr, "Ошибка открытия входного файла\n");
        return 1;
    }

    if (output_file == NULL) {
        fprintf(stderr, "Ошибка открытия выходного файла\n");
        fclose(input_file);
        return 1;
    }

    char current_word[MAX_WORD_LENGTH];
    char previous_word[MAX_WORD_LENGTH];
    int count = 0;

    // Инициализация предыдущего слова пустой строкой
    previous_word[0] = '\0';

    // Чтение слов из файла
    while (fscanf(input_file, "%s", current_word) != EOF) {
        // Если текущее слово совпадает с предыдущим, увеличиваем счётчик
        if (str_case_cmp(current_word, previous_word) == 0) {
            count++;
        } else {
            // Если предыдущее слово не пустое, записываем его и счётчик в файл
            if (previous_word[0] != '\0') {
                fprintf(output_file, "%s(%d)\n", previous_word, count);
            }
            // Обновляем предыдущее слово и сбрасываем счётчик
            strcpy(previous_word, current_word);
            count = 1;
        }
    }

    // Записываем последнее слово и его счётчик
    if (previous_word[0] != '\0') {
        fprintf(output_file, "%s(%d)\n", previous_word, count);
    }

    // Закрываем файлы
    fclose(input_file);
    fclose(output_file);

    printf("Обработка завершена. Результат записан в output.txt\n");

    return 0;
}