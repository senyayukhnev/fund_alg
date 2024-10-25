#include <stdio.h>
#include <math.h>
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
//double func_3(int n, double x){
//    return (pow(3, 3*n) * pow(factorial(n), 3) * pow(x, 2*n)) / factorial(3 * n);
//}
//double func_4(int n, double x){
//    return (pow(-1, n) * factorial_2(2 * n - 1) * pow(x, 2 * n)) / factorial_2(2 * n);
//}
int CheckDoubleArgs(double *arg){
    double me = 1;
    while (1 + me * 0.5 > 1) me *= 0.5;
    for(int i = 0; arg[i]; i++) {
        if (me > fabs(arg[i])) return 1;

    }
    return 0;
}
int HandlerOptT(double* arg) {
    double eps = arg[0];
    double a = arg[1];
    double b = arg[2];
    double c = arg[3];
    int is_triangle = 0;

    if (a + b > c || a + c > b || b + c > a) {
        double pot_hyp, cat1, cat2;

        if (a > b && a > c) {
            pot_hyp = a;
            cat1 = b;
            cat2 = c;
        } else if (b > a && b > c) {
            pot_hyp = b;
            cat1 = a;
            cat2 = c;
        } else {
            pot_hyp = c;
            cat1 = a;
            cat2 = b;
        }
        double pif = sqrt(cat1 * cat1 + cat2 * cat2);
        if (cat1 * cat1 < 0 || cat2 * cat2 < 0) {
            return 100;
        } else {
            if (fabs(pot_hyp - pif) <= eps) {
                is_triangle = 1;
            }
        }
    }
    return is_triangle;
}

int main(int argc, char** argv) {
    double arg[5] = {1, 1, 1};
    printf("%d", HandlerOptT(arg));
}
