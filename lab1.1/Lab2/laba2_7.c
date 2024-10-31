#include <stdio.h>
#include <math.h>


double ln2_equ(double x){
    return pow(x - 1, 2) / (x - 1);
}

double exp_equ(double x){
    return log(6*x - 17) - 4;
}

double dichotomy(double left_b, double right_b, double eps, double (*func)(double x)){
    double c = 1;
    while (fabs(right_b - left_b) > eps){
        c = (right_b + left_b) * 0.5;
        if (func(c) * func(left_b) > 0){
            left_b = c;
        }
        else{
            right_b = c;
        }
    }
    return c;
}

int main(int argc, char** argv){
    double eps1 = 0.00001;
    double eps2 = 2;
    double eps3 = 0.00000000001;

    printf("Equation 1 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 0.0, 1.0, eps1, dichotomy(0.0, 1.0, eps1, ln2_equ));
    printf("Equation 1 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 2.0, 3.0, eps1, dichotomy(2.0, 3.0, eps1, ln2_equ));
    printf("Equation 1 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 0.0, -1.0, eps1, dichotomy(0.0, -1.0, eps1, ln2_equ));
    printf("Equation 1 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 0.0, 1.0, eps2, dichotomy(0.0, 1.0, eps2, ln2_equ));
    printf("Equation 1 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 0.0, 1.0, eps3, dichotomy(0.0, 1.0, eps3, ln2_equ));
    printf("\n");
    printf("Equation 2 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 0.0, 1.0, eps1, dichotomy(0.0, 1.0, eps1, exp_equ));
    printf("Equation 2 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 2.0, 3.0, eps1, dichotomy(2.0, 3.0, eps1, exp_equ));
    printf("Equation 2 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 11.0, 12.0, eps1, dichotomy(11.0, 12.0, eps1, exp_equ));
    printf("Equation 2 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 3.0, 5000.0, eps2, dichotomy(3.0, 50000.0, eps2, exp_equ));
    printf("Equation 2 for [%lf, %lf] for eps = %.10lf: %.10lf\n", 5.0, 150.0, eps3, dichotomy(5.0, 150.0, eps3, exp_equ));

    return 0;
}