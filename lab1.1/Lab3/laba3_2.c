#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdarg.h"

typedef struct Matrix {
    int n;
    double *data;
} Matrix;
typedef enum Err{
    Normal,
    Diff_Dimension
}Err;

typedef struct Vector{
    int n;
    double* coor;
}Vector;


Vector* create_vector(int n, ...){
//    if (n < 1) return NULL;
    Vector *vector = (Vector*)malloc(sizeof(Vector));
    if (!vector) return NULL;

    vector->n = n;
    vector->coor = (double*)malloc(sizeof(double) * n);
    if (!vector->coor){
        free(vector);
        return NULL;
    }

    va_list args;
    va_start(args, n);
    for(int i = 0; i < n; i++){
        vector->coor[i] = va_arg(args, double);
    }
    va_end(args);

    return vector;
}
double max(int count, Vector arr[], Vector*** max_vectors, int* max_count) {
    if (count < 1) return Diff_Dimension;

    double mx_len = 0;
    int max_len_count = 0;

    // Первый проход для нахождения максимальной длины
    for (int i = 0; i < count; i++) {
        if (arr[0].n != arr[i].n) {
            return Diff_Dimension;
        }

        double len = 0;
        for (int j = 0; j < arr[i].n; j++) {
            len += arr[i].coor[j] * arr[i].coor[j];
        }
        len = sqrt(len);

        if (len > mx_len) {
            mx_len = len;
            max_len_count = 1;
        } else if (len == mx_len) {
            max_len_count++;
        }
    }

    // Выделение памяти для указателей на векторы с максимальной длиной
    *max_vectors = (Vector**)malloc(sizeof(Vector*) * max_len_count);
    if (!*max_vectors) return Diff_Dimension;

    // Второй проход для записи всех векторов с максимальной длиной
    int idx = 0;
    for (int i = 0; i < count; i++) {
        double len = 0;
        for (int j = 0; j < arr[i].n; j++) {
            len += arr[i].coor[j] * arr[i].coor[j];
        }
        len = sqrt(len);

        if (len == mx_len) {
            (*max_vectors)[idx++] = &arr[i];
        }
    }

    *max_count = max_len_count;
    return Normal;
}
void delete_vector(Vector *v){
    if (v){
        free(v->coor);
        free(v);
        v = NULL;
    }
}
double find_vectors_with_max_coordinate(int count, Vector arr[], Vector*** max_vectors, int* max_count) {
    if (count < 1) return Diff_Dimension;

    double max_coordinate = -INFINITY;
    int max_coordinate_count = 0;

    // Первый проход: находим максимальную координату среди всех векторов
    for (int i = 0; i < count; i++) {
        if (arr[0].n != arr[i].n) {
            return Diff_Dimension;
        }

        for (int j = 0; j < arr[i].n; j++) {
            if (arr[i].coor[j] > max_coordinate) {
                max_coordinate = arr[i].coor[j];
                max_coordinate_count = 1;
            } else if (arr[i].coor[j] == max_coordinate) {
                max_coordinate_count++;
            }
        }
    }

    // Выделение памяти для указателей на векторы с максимальной координатой
    *max_vectors = (Vector**)malloc(sizeof(Vector*) * max_coordinate_count);
    if (!*max_vectors) return Diff_Dimension;

    // Второй проход: заполняем массив всеми векторами с максимальной координатой
    int idx = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < arr[i].n; j++) {
            if (arr[i].coor[j] == max_coordinate) {
                (*max_vectors)[idx++] = &arr[i];
                break;  // Переходим к следующему вектору, так как найдено совпадение
            }
        }
    }

    *max_count = max_coordinate_count;
    return Normal;
}
// Функция для умножения матрицы на вектор
void matrix_vector_multiply(const Matrix *A, const Vector *x, Vector *result) {
    for (int i = 0; i < A->n; i++) {
        result->coor[i] = 0;
        for (int j = 0; j < A->n; j++) {
            result->coor[i] += A->data[i * A->n + j] * x->coor[j];
        }
    }
}

// Функция для вычисления скалярного произведения двух векторов
double dot_product(const Vector *x, const Vector *y) {
    double sum = 0;
    for (int i = 0; i < x->n; i++) {
        sum += x->coor[i] * y->coor[i];
    }
    return sum;
}

// Функция для вычисления нормы ||x||_A = sqrt((Ax, x))
double compute_norm(const Matrix *A, const Vector *x, void (*mat_vec_mult)(const Matrix*, const Vector*, Vector*), double (*dot)(const Vector*, const Vector*)) {
    Vector Ax = {x->n, malloc(x->n * sizeof(double))};
    if (!Ax.coor) return -1;

    // Вычисляем Ax
    mat_vec_mult(A, x, &Ax);

    // Вычисляем скалярное произведение (Ax, x)
    double result = dot(&Ax, x);

    // Освобождаем память для Ax
    free(Ax.coor);

    // Возвращаем корень квадратный от результата
    return sqrt(result);
}

int main() {
    Vector *v = create_vector(3, 1.0, 2.0, 3.0);
    Vector *v1 = create_vector(3, 3.0, 4.0, 5.0);
    Vector *v2 = create_vector(3, 5.0, 6.0, 7.0);
    Vector *v3 = create_vector(3, 5.0, 6.0, 7.0);  // Вектор с той же длиной, что и v2

    if (!v || !v1 || !v2 || !v3) {
        printf("Memory allocation error!\n");
        delete_vector(v);
        delete_vector(v1);
        delete_vector(v2);
        delete_vector(v3);
        return 1;
    }

    // Массив векторов
    Vector array[] = {*v, *v1, *v2, *v3};

    Vector** max_vectors = NULL;
    int max_count = 0;

    // Вызов функции max и проверка результата
    if (max(4, array, &max_vectors, &max_count) == Normal) {
        printf("Number of longest vectors: %d\n", max_count);
        for (int i = 0; i < max_count; i++) {
            printf("Vector %d: ", i + 1);
            for (int j = 0; j < max_vectors[i]->n; j++) {
                printf("%lf ", max_vectors[i]->coor[j]);
            }
            printf("\n");
        }
    } else {
        printf("Vectors have different dimensions!\n");
    }
    if (find_vectors_with_max_coordinate(4, array, &max_vectors, &max_count) == Normal) {
        printf("Number of vectors with the highest coordinate: %d\n", max_count);
        for (int i = 0; i < max_count; i++) {
            printf("Vector %d: ", i + 1);
            for (int j = 0; j < max_vectors[i]->n; j++) {
                printf("%lf ", max_vectors[i]->coor[j]);
            }
            printf("\n");
        }
    } else {
        printf("Vectors have different dimensions!\n");
    }
    //3 норма
    double A_data[] = {4, 1, 1, 3};
    Matrix A = {2, A_data};

    // Вектор x
    double x_data[] = {1, 2};
    Vector x = {2, x_data};

    // Вычисление нормы ||x||_A
    double norm = compute_norm(&A, &x, matrix_vector_multiply, dot_product);
    if (norm >= 0) {
        printf("||x||_A = %lf\n", norm);
    } else {
        printf("Err to calc norm 3.\n");
    }

    // Освобождаем память
    delete_vector(v);
    delete_vector(v1);
    delete_vector(v2);
    delete_vector(v3);
    free(max_vectors);  // Освобождаем память для массива указателей на векторы

    return 0;
}
