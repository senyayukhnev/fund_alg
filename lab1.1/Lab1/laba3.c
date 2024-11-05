#include <stdio.h>
#include <math.h>
#include "limits.h"
typedef enum kOpts {
    OPT_Q,
    OPT_M,
    OPT_T,
} kOpts;
int GetIntArg(char** argv, int arg_ind){
    int to_save = 0;
    int sign = 1;
    int start_point = 0;
    if (argv[arg_ind][0] == '-'){
        sign = -1;
        start_point += 1;
    }
    for (int i = start_point; argv[arg_ind][i]; i++) {
        if ('9' >= argv[arg_ind][i] && argv[arg_ind][i] >= '0') {
            to_save *= 10;
            to_save += (argv[arg_ind][i] - '0');
        }
        else{
            return 0;
        }
    }
    if (to_save < 1 && to_save != 0) return 1;
    return sign * to_save;
}
int CheckIntArgs(int *argi){
    for(int i = 0; argi[i]; i++) {
        if (fabs(argi[i]) > INT_MAX) return 1;
    }
    return 0;
}
//double GetDoubleArg(char** argv, int arg_ind){
//    double to_save = 0.0;
//    int sign = 1;
//    int dot_ind = 0;
//    int start_point = 0;
//    if (argv[arg_ind][0] == '-'){
//        sign = -1;
//        start_point += 1;
//    }
//    for (int i = start_point; argv[arg_ind][i]; i++) {
//        if (argv[arg_ind][i] == ',') {
//            return 1;
//        }
//
//        if (argv[arg_ind][i] == '.') {
//            dot_ind = i;
//            break;
//        }
//
//        if ('9' >= argv[arg_ind][i] && argv[arg_ind][i] >= '0') {
//            to_save *= 10;
//            to_save += (argv[arg_ind][i] - '0');
//        }
//        else{
//            return 1;
//        }
//    }
//
//    for (int i = dot_ind + 1 + start_point; argv[arg_ind][i]; i++) {
//        to_save += ((argv[arg_ind][i] - '0') / pow(10, i - dot_ind));
//    }
//    return sign * to_save;
//}
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
        if (me > fabs(arg[i]) || fabs(arg[i]) > ULONG_LONG_MAX) return 1;
    }
    return 0;
}
int GetOpts(int argc, char** argv, kOpts* option, double arg[5], int argi[3]) {
    char *flag = argv[1];
    if ((flag[0] != '-') && (flag[0] != '/')) {
        return 1;
    }
    if (flag[2]){
        return 1;
    }
    switch (flag[1])
    {
        case 'q':
            printf("Detected flag: OPT_Q\n");
            *option = OPT_Q;
            if (argc != 6) {
                return 1;
            }
            arg[0] = GetDoubleArg(argv, 2);
            if (arg[0] <= 0.0) {
                return 1;
            }
            arg[1] = GetDoubleArg(argv, 3);
            arg[2] = GetDoubleArg(argv, 4);
            arg[3] = GetDoubleArg(argv, 5);
            arg[4] = 0;
            if (CheckDoubleArgs(arg)) return 1;
            break;
        case 'm':
            *option = OPT_M;
            if (argc != 4) return 1;
            argi[0] = GetIntArg(argv, 2);
            argi[1] = GetIntArg(argv, 3);
            if (argi[0] <= 0 || argi[1] <= 0){
                return 1;
            }
            argi[2] = 0;
            if (CheckIntArgs(argi)) return 1;
            break;
        case 't':
            *option = OPT_T;

            if (argc != 6) return 1;
            arg[0] = GetDoubleArg(argv, 2);
            arg[1] = GetDoubleArg(argv, 3);
            arg[2] = GetDoubleArg(argv, 4);
            arg[3] = GetDoubleArg(argv, 5);

            if (arg[0] <= 0.0 || arg[1] <= 0.0 || arg[2] <= 0.0 || arg[3] <= 0.0) {
                return 1;
            }
            arg[4] = 0;
            if (CheckDoubleArgs(arg)) return 1;
            break;
        default:
            printf("Detected flag: unknown\n");
            return 1;
    }
    return 0;
}
double solve_lineal_eq(double b, double c, double ans[]){
    double x = c / b;
    ans[0] = x;
    ans[1] = 0;
    return 0;
}
double solve_quadratic_eq(double a, double b, double c, double eps, double ans[]){
    if (b * b < 0 || 4 * fabs(a) * fabs(c) < 0){
        return 100;
    }
    double descriminant = b * b - 4 * a * c;
    if (a == 0 && b == 0 && c == 0) return 103; //infinity roots
    if (a == 0){
        solve_lineal_eq(b, c, ans);
        return 1;
    }
    if (fabs(descriminant) <= eps){
        double x = -b / (2 * a);
        ans[0] = x;
        return 1;
    }else if (descriminant > eps){
        double x1 = (-b + sqrt(descriminant)) / (2 * a);
        double x2 = (-b + sqrt(descriminant)) / (2 * a);
        ans[0] = x1;
        ans[1] = x2;
        return 2;
    }
    else{
        return 0; // only complex roots
    }
}
int HandlerOptQ(double arg[], double num_root[6], double ans[6][2]){
    double eps = arg[0];
    double a = arg[1];
    double b = arg[2];
    double c = arg[3];
    double coeffs[3] = {a, b, c};

    double permutations[6][3]; /* = {
            {coeffs[0], coeffs[1], coeffs[2]},
            {coeffs[0], coeffs[2], coeffs[1]},
            {coeffs[1], coeffs[0], coeffs[2]},
            {coeffs[1], coeffs[2], coeffs[0]},
            {coeffs[2], coeffs[0], coeffs[1]},
            {coeffs[2], coeffs[1], coeffs[0]}
    };*/
    double x = 0;
    double y = 0;
    double z = 0;
    for(int i = 0; i < 3; i++){
        permutations[i][0] = coeffs[0];
        permutations[i][1] = coeffs[1];
        permutations[i][2] = coeffs[2];
        z = coeffs[0];
        x = coeffs[1];
        y = coeffs[2];
        coeffs[0] = x;
        coeffs[1] = y;
        coeffs[2] = z;
    }
    for(int i = 3; i < 6; i++){
        permutations[i][0] = permutations[5 - i][2];
        permutations[i][1] = permutations[5 - i][1];
        permutations[i][2] = permutations[5 - i][0];
    }
    for (int i = 0; i < 6; i++){
        num_root[i] = solve_quadratic_eq(permutations[i][0], permutations[i][1], permutations[i][2], eps, ans[i]);
    }
    return 0;
}
int HandlerOptM(int* arg){
    int a = arg[0];
    int b = arg[1];
    if (a % b == 0){
        return 1;
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
    kOpts opt;
    double arg[5];
    int argi[3];
    int res = GetOpts(argc, argv, &opt, arg, argi);
    if (res == 1) {
        printf("%s", "Incorrect input");
        return 1;

    }
    switch (opt) {
        case OPT_Q:
            double ans[6][2];
            double num_root[6];
            double eps = arg[0];
            double a = arg[1];
            double b = arg[2];
            double c = arg[3];
            HandlerOptQ(arg, num_root, ans);

            double permutations[6][3];
            double x = 0;
            double y = 0;
            double z = 0;
            for(int i = 0; i < 3; i++){
                permutations[i][0] = a;
                permutations[i][1] = b;
                permutations[i][2] = c;
                z = a;
                x = b;
                y = c;
                a = x;
                b = y;
                c = z;
            }
            for(int i = 3; i < 6; i++){
                permutations[i][0] = permutations[5 - i][2];
                permutations[i][1] = permutations[5 - i][1];
                permutations[i][2] = permutations[5 - i][0];
            }
            /* = {
                    {a, b, c},
                    {a, c, b},
                    {b, a, c},
                    {b, c, a},
                    {c, a, b},
                    {c, b, a}
            };*/

            double right_permutation[6][3];
            int len = 0;
            for (int i = 0; i < 6; i++) {
                int f = 1;
                for (int j = i + 1; j < 6; j++) {
                    if (permutations[i][0] == permutations[j][0] && permutations[i][1] == permutations[j][1]) {
                        f = 0;
                    }
                }
                if (f) {
                    right_permutation[len][0] = permutations[i][0];
                    right_permutation[len][1] = permutations[i][1];
                    right_permutation[len][2] = permutations[i][2];
                    len++;
                }
            }
            for (int i = 0; i < len; i++) {
                printf("Solving for a = %.2lf, b = %.2lf, c = %.2lf:\n",
                       right_permutation[i][0], right_permutation[i][1], right_permutation[i][2]);

                if (num_root[i] == 2) {
                    printf("x1 = %.10lf, x2 = %.10lf\n", ans[i][0], ans[i][1]);
                } else if (num_root[i] == 1) {
                    printf("x = %.10lf\n", ans[i][0]);
                } else if (num_root[i] == 103) {
                    printf("infinity roots\n");
                } else if (num_root[i] == 100) {
                    printf("Descriminant overflow");
                } else {
                    printf("only complex roots\n");
                }
            }
            return 0;
        case OPT_M:
            if (HandlerOptM(argi) == 1) {
                printf("KRatNo");
            } else {
                printf("Ne Kratno");
            }
            return 0;
        case OPT_T:
            printf("%lf %lf %lf %lf\n", arg[0], arg[1], arg[2], arg[3]);
            if (HandlerOptT(arg) == 1) {
                printf("True");
            } else if (HandlerOptT(arg) == 100) {
                printf("Overflow pif");
            } else {
                printf("False");
            }
            break;
    }
    return 0;
}