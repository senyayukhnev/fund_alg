#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>

// Структура для предиката
typedef struct {
    int (*func)(int); // Указатель на функцию-предикат
    float coeff;      // Весовой коэффициент
} Predicate;

// Преобразователь выходного значения предиката
int fix(int value) {
    return (value == 0) ? 0 : 1;
}

// Предикаты для демонстрации
int is_even(int value) {
    return value % 2 == 0;
}

int is_positive(int value) {
    return value > 0;
}

int is_greater_than_five(int value) {
    return value > 5;
}

// Функция для вычисления значения ordering
float calculate_ordering(int item, Predicate* predicates, size_t pred_count) {
    float ordering = 0.0f;
    for (size_t i = 0; i < pred_count; i++) {
        ordering += fix(predicates[i].func(item)) * predicates[i].coeff;
    }
    return ordering;
}

// Предикат для qsort
int compare_ordering(const void* a, const void* b) {
    const float* ordering_a = (const float*)a;
    const float* ordering_b = (const float*)b;
    return (*ordering_a > *ordering_b) - (*ordering_a < *ordering_b);
}

// Основная функция сортировки
int sort_by_predicates(int* values, size_t values_count, int** result, int ascending, ...) {
    if (values == NULL || values_count == 0 || result == NULL) {
        return -1; // Некорректные входные данные
    }

    // Выделение памяти для результирующего массива
    *result = (int*)malloc(values_count * sizeof(int));
    if (*result == NULL) {
        return -1; // Ошибка выделения памяти
    }

    // Копирование исходного массива в результирующий
    for (size_t i = 0; i < values_count; i++) {
        (*result)[i] = values[i];
    }

    // Инициализация списка аргументов переменной длины
    va_list args;
    va_start(args, ascending);

    // Сбор предикатов
    Predicate predicates[100]; // Максимальное количество предикатов
    size_t pred_count = 0;

    while (1) {
        Predicate* pred = va_arg(args, Predicate*);
        if (pred == NULL) {
            break; // Конец списка предикатов
        }
        if (pred->coeff < 0.01f || pred->coeff > 5.00f) {
            return -1; // Некорректный весовой коэффициент
        }
        predicates[pred_count++] = *pred;
    }

    va_end(args);

    // Вычисление значений ordering для каждого элемента
    float* orderings = (float*)malloc(values_count * sizeof(float));
    if (orderings == NULL) {
        free(*result);
        return -1; // Ошибка выделения памяти
    }

    for (size_t i = 0; i < values_count; i++) {
        orderings[i] = calculate_ordering((*result)[i], predicates, pred_count);
    }

    // Сортировка массива по значениям ordering
    qsort(orderings, values_count, sizeof(float), compare_ordering);

    // Создание отсортированного массива
    int* sorted_result = (int*)malloc(values_count * sizeof(int));
    if (sorted_result == NULL) {
        free(*result);
        free(orderings);
        return -1; // Ошибка выделения памяти
    }

    for (size_t i = 0; i < values_count; i++) {
        for (size_t j = 0; j < values_count; j++) {
            if (orderings[i] == calculate_ordering((*result)[j], predicates, pred_count)) {
                sorted_result[i] = (*result)[j];
                break;
            }
        }
    }

    // Освобождение памяти
    free(*result);
    free(orderings);

    // Возвращение отсортированного массива
    *result = sorted_result;

    return 0; // Успешное выполнение
}

int main() {
    int values[] = {3, -1, 7, 4, -5, 6};
    size_t values_count = sizeof(values) / sizeof(values[0]);
    int* result = NULL;

    // Предикаты
    Predicate pred1 = {is_even, 1.0f};
    Predicate pred2 = {is_positive, 2.0f};
    Predicate pred3 = {is_greater_than_five, 3.0f};

    // Сортировка по возрастанию
    int status = sort_by_predicates(values, values_count, &result, 1, &pred1, &pred2, &pred3, NULL);

    if (status == 0) {
        printf("Отсортированный массив: ");
        for (size_t i = 0; i < values_count; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("Ошибка сортировки\n");
    }

    return 0;
}