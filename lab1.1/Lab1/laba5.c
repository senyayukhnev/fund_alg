#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

double GetDoubleArg(char** argv, int arg_ind){
    double to_save = 0.0;
    int sign = 1;
    int dot_ind = 0;
    int start_point = 0;
    int f = 0;
    int after_dot = 0;
    if (argv[arg_ind][0] == '-'){
        sign = -1;
        start_point += 1;
    }
    for (int i = start_point; argv[arg_ind][i]; i++) {
        if (argv[arg_ind][i] == ',') {
            return 1;
        }

        if (argv[arg_ind][i] == '.') {
            f = 1;
            continue;
        }

        if ('9' >= argv[arg_ind][i] && argv[arg_ind][i] >= '0') {
            to_save *= 10;
            to_save += (argv[arg_ind][i] - '0');
            if (f) {
                after_dot+=1;

            }
        }
        else{
            return 1;
        }
    }

    to_save = to_save / pow(10, after_dot);
    return sign * to_save;
}
int CheckDoubleArgs(double *arg){
    double me = 1;
    while (1 + me * 0.5 > 1) me *= 0.5;
    for(int i = 0; arg[i]; i++) {
        if (me > fabs(arg[i]) || fabs(arg[i]) > ULLONG_MAX) return 1;
    }
    return 0;
}
int GetEps(int argc, char** argv, double* eps, double* x) {
    if (argc != 3) {
        return 1;
    }
    *eps = GetDoubleArg(argv, 1);
    *x = GetDoubleArg(argv, 2);
    double arg[] = {*eps, *x, 0};
    if (CheckDoubleArgs(arg)){
        return 1;
    }
    if (*eps <= 0.0 ) {
        return 1;
    }
    return 0;

    /*int dot_ind = 0;
    for (int i = 0; argv[1][i]; i++) {
        if (argv[1][i] == ',') {
            return 1;

        }

        if (argv[1][i] == '.') {
            dot_ind = i;
            break;
        }
        *eps += (argv[1][i] - '0');

    }

    for (int i = dot_ind + 1; argv[1][i]; i++) {
        *eps += ((argv[1][i] - '0') / pow(10, i - dot_ind));
    }
    if (*eps <= 0.0 ) {
        return 1;
    }
    double me = 1;
    while (1 + me * 0.5 > 1) me *= 0.5;
    if (me > *eps) return 1;
    dot_ind = 0;
    for (int i = 0; argv[2][i]; i++) {
        if (argv[2][i] == ',') {
            return 1;
        }

        if (argv[2][i] == '.') {
            dot_ind = i;
            break;
        }
        *x += (argv[2][i] - '0');

    }
    for (int i = dot_ind + 1; argv[2][i]; i++) {
        *x += ((argv[2][i] - '0') / pow(10, i - dot_ind));
    }
    if (me > fabs(*x)) return 1;
    return 0;
     */
}

//double factorial(double n){
//    double temp = 1;
//    for(int i = 1; i <= n; i++) temp *= i;
//    return temp;
//}
//double factorial_2(double n) {
//    if (n == 0 || n == 1) return 1;
//    double temp = 1;
//    for (int x = n; x >= 1; x -= 2) temp *= x;
//    return temp;
//}

double mul_gen(int start_val, double (*func)(int n, double x), double eps, double x, double first_val) {
    double val = first_val;

    double sum = first_val;// 1
    double prev_sum = 0.0;
    for(int i = start_val + 1;fabs(sum - prev_sum) >= eps ;i++){
        prev_sum = sum;
        val *= func(i - 1, x);
        sum += val;
        }
    return sum;
}

double func_1(int n, double x){
    return x / (n + 1);
}
double func_2(int n, double x){
    return (-1 * pow(x, 2)) / ((2 * n + 1) * (2 * n + 2));
}
double func_3(int n, double x){
    return (27 * pow(n + 1, 3) * pow(x, 2)) / ((3 * n + 1) * (3 * n + 2) * (3 * n + 3));
}
double func_4(int n, double x){
    return (-1.0 * (2.0 * n + 1.0) * x * x) / (2.0 * n + 2.0);
}
//double func_1(int n, double x){
//    return pow(x, n) / factorial(n);
//}
//double func_2(int n, double x){
//    return (pow(-1, n) * pow(x, 2*n)) / factorial(2 * n);
//}
//double func_3(int n, double x){
//    return (pow(3, 3*n) * pow(factorial(n), 3) * pow(x, 2*n)) / factorial(3 * n);
//}
//double func_4(int n, double x){
//    return (pow(-1, n) * factorial_2(2 * n - 1) * pow(x, 2 * n)) / factorial_2(2 * n);
//}

double series(double eps, double x, double (*func)(double eps, double x)) {
    return func(eps, x);
}
double series_1(double eps, double x) {
    int start_val = 0;
    double first_val = 1;
    return mul_gen(start_val, func_1, eps, x, first_val);
}

double series_2(double eps, double x) {
    int start_val = 0;
    double first_val = 1;
    return mul_gen(start_val, func_2, eps, x, first_val);
}
double series_3(double eps, double x){
    int start_val = 0;
    double first_val = 1.0;
    if (x >= 1){
        return -100;
    }
    return mul_gen(start_val, func_3, eps, x, first_val);
}
double series_4(double eps, double x){
    int start_val = 1;
    double first_val = (-1.0 * 1 * x * x) / 2.0;
    if(x > 1){
        return -100;
    }
    return mul_gen(start_val, func_4, eps, x, first_val);
}
int main(int argc, char** argv) {
    double eps = 0;
    double x = 0;
    if (GetEps(argc, argv, &eps, &x)) {
        printf("%s", "Incorrect input");
        return 1;
    }
    printf("Eps: %.9lf \n", eps);
    printf("X: %lf \n", x);
    printf("1 serie: %lf \n", series(eps, x, series_1));
    printf("2 serie: %lf \n", series(eps, x, series_2));
    if (series_3(eps, x) == -100){
        printf("3: baaad\n");
    }
    else{
        printf("3 serie: %lf \n", series(eps, x, series_3));
    }
    if (series_4(eps, x) == -100){
        printf("4: baaad\n");
    }
    else {
        printf("4 serie: %lf \n", series(eps, x, series_4));
    }
    return 0;
}
