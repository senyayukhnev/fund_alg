#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

typedef enum {
    Normal,
    Memory_allocate_err,
} ResultCode;

void reportError(ResultCode code) {
    switch (code) {
        case Normal:
            printf("Operation completed successfully!\n");
            break;
        case Memory_allocate_err:
            printf("Memory allocation failed!\n");
            break;
    }
}

// Вычисляет i-ю производную многочлена в точке x, где n - его степень
double computeDerivative(int order, double point, int degree, const double *polynomial) {
    double sum = 0.0;

    for (int j = order; j <= degree; j++) {
        double term = polynomial[j];
        for (int k = 0; k < order; k++) {
            term *= (j - k);
        }
        sum += term * pow(point, j - order);
    }
    return sum;
}

ResultCode decompose(double tolerance, double center, double *out_terms, int degree, ...) {
    if (!out_terms) return -1;
    va_list arguments;
    va_start(arguments, degree);

    double *polynomial = (double *) malloc((degree + 1) * sizeof(double));
    if (!polynomial) return Memory_allocate_err;

    for (int i = 0; i <= degree; i++) {
        polynomial[i] = va_arg(arguments, double);
    }
    va_end(arguments);

    double factorial = 1.0;
    for (int i = 0; i <= degree; i++) {
        if (i != 0) {
            factorial *= (double) i;
        }
        out_terms[i] = computeDerivative(i, center, degree, polynomial) / factorial;
        if (fabs(out_terms[i]) < tolerance) {
            out_terms[i] = 0.0;
        }
    }

    free(polynomial);
    return Normal;
}

int main() {
    int degree = 2;
    double tolerance = 1e-6;
    double center = 1.0;
    double* terms = malloc((degree+1) * sizeof(double));
    if(!terms) {
        reportError(Memory_allocate_err);
        return Memory_allocate_err;
    }

    ResultCode code = decompose(tolerance, center, terms, degree, 2.0, 3.0, 4.0);
    if (code != Normal) {
        reportError(code);
        return code;
    }

    for (int i = 0; i <= degree; i++) {
        printf("g%d = %f\n", i, terms[i]);
    }
    free(terms);
    return Normal;
}
