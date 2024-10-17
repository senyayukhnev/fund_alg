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
        *eps *= 10;
        *eps += (argv[1][i] - '0');

    }
    for (int i = dot_ind + 1; argv[1][i]; i++) {
        *eps += ((argv[1][i] - '0') / pow(10, i - dot_ind));
    }
    if (*eps <= 0.0) {
        return 1;
    }
    double me = 1;
    while (1 + me * 0.5 > 1) me *= 0.5;
    if (me > *eps) return 1;
    if (*eps <= 0.00000001) return 101;
    return 0;
}

double factorial(double n){
    double temp = 1;
    for(int i = 1; i <= n; i++) temp *= i;
    return temp;
}


double sum_gen(int start_val, double (*func)(int), double eps) {
    double prev_sum = 0.0;
    double current_sum = func(start_val);
    for (int i = start_val + 1; ; i++) {
        prev_sum = current_sum;
        current_sum += func(i);

        if (fabs(current_sum - prev_sum) < eps) {
            return current_sum;
        }
    }
}
double mul_gen(int start_val, double (*func)(int), double eps) {
    double prev_mul = 0.0;
    double current_mul = func(start_val);
    for (int i = start_val + 1; ; i++) {
        prev_mul = current_mul;
        current_mul *= func(i);

        if (fabs(current_mul - prev_mul) < eps) {
            return current_mul;
        }
    }
}
double rt2(int n){
    return pow(2, pow(2, -n));
}
double ln2(int n){
    return pow(-1, n - 1) / n;
}
double e(int n) {
    return 1 / factorial(n);
}
double pi(int n) {
    return (4 * pow(-1, n - 1)) / (2 * n - 1);
}
double gamma(int n){
    return 1.0 / pow(floor(sqrt(n)), 2.0) - 1.0 / n;
}
double series_e(double eps) {
    int start_val = 0;
    return sum_gen(start_val, e, eps);
}
double series_gamma(double eps){
    int start_val = 2;
    return -(pi(1) * pi(1) / 6) + sum_gen(start_val, gamma, eps);
}
double series_pi(double eps) {
    int start_val = 1;
    return sum_gen(start_val, pi, eps);
}
double series_ln2(double eps){
    int start_val = 1;
    return sum_gen(start_val, ln2, eps);
}
double series_rt2(double eps){
    int start_val = 2;
    return mul_gen(start_val, rt2, eps);
}
double series(double eps, double (*func)(double n)) {
    return func(eps);
}
double lim_ln2(double n){
    return n * (pow(2, 1/n) - 1);
}
double lim_e(double n){
    return pow(1 + 1/n, n);
}
// (pow(pow(2, n) * factorial(n), 4) / (n * pow(factorial(2 * n), 2)));
double lim_pi(double n){
    return (pow(pow(2, n) * factorial(n), 4.0) / (n * pow(factorial(2*n), 2.0))); //16 * pow(n + 1, 2) * n;
}
double lim_rt2(double n){
    double prev_x;
    double cur_x = -0.5;
    for(int i = 1; i <= n; i++){
        prev_x = cur_x;
        cur_x = prev_x - pow(prev_x, 2) / 2 + 1;
    }
    return cur_x;
}


double lim(double eps, double (*func)(double n)){
    double prev_val = func(1);
    double val = func(2);
    double n = 2;
    while (fabs(val - prev_val) > eps){
        prev_val = val;
        n++;
        val = func(n);
    }
    return prev_val;
}
double dih_e(double x){
    return log(x) - 1;
}
double dih_pi(double x){
    return sin(x);
}
double dih_ln2(double x){
    return exp(x) - 2;
}
double dih_rt2(double x){
    return pow(x, 2) - 2;
}

double nm_pi(double x){
    return cos(x) + 1;
}
double deriv_pi(double x){
    return -sin(x);
}
double newton_method(double (*func)(double), double (*deriv)(double), double x0, double epsilon) {
    double x = x0;
    for (int i = 0; ; i++) {
        double x_new = x - func(x) / deriv(x);
        if (fabs(x_new - x) < epsilon) {
            return x_new;
        }
        x = x_new;
    }
}
double dih(double eps, double (*func)(double x), double left_b, double right_b){
    double c = (left_b + right_b) / 2;
    while (fabs(func(c)) > eps){
        if (func(c) * func(left_b) < 0){
            right_b = c;
        }
        if (func(c) * func(right_b) < 0){
            left_b = c;
        }
        c = (right_b + left_b) / 2;
    }
    return c;
}

int main(int argc, char** argv)
{
    double eps = 0;
    int res = GetEps(argc, argv, &eps);
    if (res == 1) {
        printf("%s", "Incorrect input");
        return 1;
    }
    if (res == 101) {
        printf("%s", "Too small epsilon. Calc time more than 10 sec");
        return 1;
    }
    printf("Eps: %lf\n", eps);
    printf("Series: \n");
    printf("e: %lf \n", series(eps, series_e));
    printf("pi: %lf \n", series(eps, series_pi));
    printf("ln2: %lf \n", series(eps, series_ln2));
    printf("rt2: %lf \n", series(eps, series_rt2));
   // printf("gamma: %lf \n", series(eps, series_gamma));

    printf("Limits: \n");
    printf("e: %lf \n", lim(eps, lim_e));
    printf("pi: %lf \n", lim(eps, lim_pi));
    printf("ln2: %lf \n", lim(eps, lim_ln2));
    printf("rt2: %lf \n", lim(eps, lim_rt2));
    printf("Dihtomia: \n");
    printf("e: %lf \n", dih(eps, dih_e, 2, 3));
    //printf("pi: %lf \n", dih(eps, dih_pi, 3, 4));
    printf("ln2: %lf \n", dih(eps, dih_ln2, 0, 1));
    printf("rt2: %lf \n", dih(eps, dih_rt2, 1, 2));
    printf("Newton:\n");
    printf("pi: %lf \n", newton_method(nm_pi, deriv_pi, 3, eps));






    return 0;
}