#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Normal,
    Invalid_argument,
    Overflow,
    Memory_allocate_err,
} Err;

void reportError(Err status) {
    switch (status) {
        case Invalid_argument:
            printf("Error: Invalid argument\n");
            break;
        case Overflow:
            printf("Error: Overflow\n");
            break;
        case Memory_allocate_err:
            printf("Error: Memory allocation failed\n");
            break;
        default:
            printf("Error: Unknown error code\n");
            break;
    }
}

// Вычисление направления поворота для определения выпуклости многоугольника
int calculateTurnDirection(double x1, double y1, double x2, double y2, double x3, double y3) {
    double ab_x = x2 - x1;
    double ab_y = y2 - y1;
    double bc_x = x3 - x2;
    double bc_y = y3 - y2;
    double cross_product = ab_x * bc_y - ab_y * bc_x;

    return (cross_product > 0.0) ? 1 : (cross_product < 0.0) ? -1 : 0;
}

// Проверка, является ли многоугольник выпуклым
bool isPolygonConvex(int pointCount, ...) {
    if (pointCount < 3) return false;
    va_list points;
    va_start(points, pointCount);

    double firstX = va_arg(points, double);
    double firstY = va_arg(points, double);
    double secondX = va_arg(points, double);
    double secondY = va_arg(points, double);
    double thirdX = va_arg(points, double);
    double thirdY = va_arg(points, double);

    int initialDirection = calculateTurnDirection(firstX, firstY, secondX, secondY, thirdX, thirdY);

    double prevX = secondX;
    double prevY = secondY;
    double currX = thirdX;
    double currY = thirdY;

    for (int i = 3; i < pointCount; i++) {
        double nextX = va_arg(points, double);
        double nextY = va_arg(points, double);

        int currentDirection = calculateTurnDirection(prevX, prevY, currX, currY, nextX, nextY);
        if (currentDirection != 0 && currentDirection != initialDirection) {
            va_end(points);
            return false;
        }

        prevX = currX;
        prevY = currY;
        currX = nextX;
        currY = nextY;
    }
    va_end(points);

    return calculateTurnDirection(prevX, prevY, currX, currY, firstX, firstY) == initialDirection &&
           calculateTurnDirection(currX, currY, firstX, firstY, secondX, secondY) == initialDirection;
}

// Вычисление значения многочлена с использованием метода Горнера
Err evaluatePolynomial(double *output, double x, int degree, ...) {
    va_list coefficients;
    va_start(coefficients, degree);

    double result = 0.0;

    for (int i = 0; i <= degree; i++) {
        double coefficient = va_arg(coefficients, double);
        result *= x;

        if (result > DBL_MAX || result < -DBL_MAX) {
            va_end(coefficients);
            return Overflow;
        }

        result += coefficient;

        if (result > DBL_MAX || result < -DBL_MAX) {
            va_end(coefficients);
            return Overflow;
        }
    }
    va_end(coefficients);
    *output = result;
    return Normal;
}

// Преобразование строки в число по заданной системе счисления
int parseFromBase(const char *str, int base) {
    int number = 0;

    while (*str) {
        char digit = *str++;
        int value = (digit >= '0' && digit <= '9') ? digit - '0' :
                    (digit >= 'A' && digit <= 'Z') ? digit - 'A' + 10 :
                    (digit >= 'a' && digit <= 'z') ? digit - 'a' + 10 : -1;

        if (value == -1 || value >= base) return -1;

        number = number * base + value;
    }
    return number;
}

// Преобразование из десятичной системы в заданную
void convertToBase(int number, int base, char *output) {
    char buffer[65];
    int index = 0;

    while (number > 0) {
        int remainder = number % base;
        buffer[index++] = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
        number /= base;
    }

    for (int i = 0; i < index; i++) {
        output[i] = buffer[index - i - 1];
    }
    output[index] = '\0';
}

// Проверка числа Капрекара в заданной системе счисления
bool isKaprekarNumber(int number, int base, Err *status) {
    if (number == 1) return true;

    int square = number * number;
    char squareStr[65];
    convertToBase(square, base, squareStr);

    int length = strlen(squareStr);
    for (int i = 1; i < length; i++) {
        int leftPart = parseFromBase(squareStr, base);
        int rightPart = parseFromBase(squareStr + i, base);

        if (rightPart > 0 && leftPart + rightPart == number) {
            return true;
        }
    }
    return false;
}

// Поиск чисел Капрекара среди введённых строк в указанной системе счисления
bool *findKaprekarNumbers(Err *status, int base, int count, ...) {
    *status = Normal;
    va_list numbers;
    va_start(numbers, count);

    bool *kaprekarResults = malloc(sizeof(bool) * count);
    if (!kaprekarResults) {
        *status = Memory_allocate_err;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        const char *numStr = va_arg(numbers, const char *);
        int decimalNumber = parseFromBase(numStr, base);

        if (decimalNumber < 0) {
            *status = Invalid_argument;
            free(kaprekarResults);
            return NULL;
        }

        kaprekarResults[i] = isKaprekarNumber(decimalNumber, base, status);
        if (*status != Normal) {
            free(kaprekarResults);
            return NULL;
        }
    }

    va_end(numbers);
    return kaprekarResults;
}

// Главная функция
int main() {
    double polynomialResult;
    Err resultCode = evaluatePolynomial(&polynomialResult, 2.0, 4, 1.0, 1.0, 1.0, 1.0, 1.0);
    if (resultCode != Normal) {
        reportError(resultCode);
        return resultCode;
    }
    printf("Polynomial result: %lf\n", polynomialResult);

    bool isConvex = isPolygonConvex(6, 1.0, 2.0, 2.0, 4.0, 4.0, 4.0, 5.0, 2.0, 3.0, 1.0, 3.0, 2.0);
    printf("The polygon is %s\n", isConvex ? "convex" : "not convex");

    return 0;
}
