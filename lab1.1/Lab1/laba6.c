#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int GetEps(int argc, char** argv, double* eps) {
    if (argc != 2) {
        return 1;
    }
    int dot_ind = 0;
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
    if (*eps <= 0.0) {
        return 1;
    }

    double me = 1;
    while (1 + me / 2 > 1) me /= 2;
    if (me > *eps) return 1;

    if (*eps < pow(10, -8)){
        return 2;
    }
    return 0;
}
double first_func(double x){
    return log(1 + x) / x;
}
double second_func(double x){
    return exp(-x * x / 2);
}
double third_func(double x){
    return log(1 / (1 - x));
}
double fourth_func(double x){
    return pow(x, x);
}
double integral(double eps, double(*func)(double x)){
    double step = eps * 0.5;
    double amount_steps = 1 / eps;
    double ans = 0;
    for(int i = 0; i < amount_steps; i++){
        ans += func(step) * eps;
        step += eps;
    }
    return ans;
}

int main(int argc, char** argv)
{
    double eps = 0;
    int res = GetEps(argc, argv, &eps);
    if (res == 1) {
        printf("%s", "Incorrect input");
        return 1;
    }
    if (res == 2){
        printf("Too small eps. Calculation time more than 15 sec.");
        return 1;
    }
    printf("Eps: %lf\n", eps);
    printf("First integral: %lf \n", integral(eps, first_func));
    printf("Second integral: %lf \n", integral(eps, second_func));
    printf("Third integral: %lf \n", integral(eps, third_func));
    printf("Fourth integral: %lf \n", integral(eps, fourth_func));

    return 0;
}