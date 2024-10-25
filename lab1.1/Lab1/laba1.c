#include <stdio.h>
#include <math.h>
#include "limits.h"

typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;

int GetOpts(int argc, char** argv, kOpts* option, int* number) {
    char* flag;
    if (argc != 3) {
        return 1;
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {


        for (int i = 0; argv[1][i]; i++) {
            if ('0' <= argv[1][i] && argv[1][i] <= '9') {
                int prev_val = *number;
                *number *= 10;
                if (*number * 0.1 != prev_val) return 1;
                *number += argv[1][i] - '0';
            }
            else {
                return 1;

            }
        }
//        if (*number == 0) {
//            return 1;
//        }
        flag = argv[2];
    }


    else {
        flag = argv[1];


        for (int i = 0; argv[2][i]; i++) {
            if ('0' <= argv[2][i] && argv[2][i] <= '9') {
                int prev_val = *number;
                *number *= 10;
                if (*number * 0.1 != prev_val) return 1;
                *number += argv[2][i] - '0';
            } else {
                return 1;
            }
        }
        if (*number == 0) {
            return 1;
        }


    }
    if ((flag[0] != '-') && (flag[0] != '/')) {
        return 1;
    }
    if (flag[2]){
        return 1;
    }
    switch (flag[1])
    {
        case 'h':
            *option = OPT_H;
            break;
        case 'p':
            *option = OPT_P;
            break;
        case 's':
            *option = OPT_S;
            break;
        case 'e':
            *option = OPT_E;
            break;
        case 'a':
            *option = OPT_A;
            break;
        case 'f':
            *option = OPT_F;
            break;
        default:
            return 1;
    }
    return 0;

}
int HandlerOptH(int number, long long ans[]) {
    int c = 0;
    int f = 1;
    if (number <= 0) return 1;
    for (int i = number; i <= 100; i += number) {
        ans[c] = i;
        c += 1;
        f = 0;
    }
    ans[c] = 0;
    if (f) return 101;
    return 0;
}

int HandlerOptP(int number, long long ans[]) {
    if (number < 1) return 1;
    if (number == 1) return 102;
    ans[0] = 1;
    ans[1] = 0;
    for (int i = 2; i <= pow(number, 0.5); i++) {
        if (number % i == 0) {
            ans[0] = 0;
            break;
        }
    }
    return 0;

}

int HandlerOptS(int number, long long ans[]) {
    if (number < 0) return 1;
    int last_ind = 0;
    for (;number >= pow(16, last_ind); last_ind++);

    for (int i = 0; i < last_ind; i++) {
        ans[last_ind - i - 1] = number % 16;
        if (ans[last_ind - i - 1] < 10){
            ans[last_ind - i - 1] = ans[last_ind - i - 1] + '0';
        }else{
            ans[last_ind - i - 1] = (ans[last_ind - i - 1] + 7) + '0';
        }
        number /= 16;
    }
    ans[last_ind] = 0;

    return 0;
}

//long long HandlerOptE(int number) {
//    int **table = NULL;
//    for (int i = 1; i <= 10; i++) {
//        int c = number * (i- 1);
//        for (int j = number; j >= 1; j--) {
//            table[c][0] = j;
//            table[c][1] = i;
//            table[c][2] = pow(i, j);
//            c += 1;
////            printf("power: %d, result for %d: %lf \n", j, i, pow(i, j));
//        }
//    }
//    return **table;
//}
int HandlerOptE(int number, long long ans[]) {
    if (number > 10 || number < 0) return 1;
    for (int i = 1; i <= 10; i++) {
        long long int n = i;
        for (int j = 1; j <= number; j++) {
            ans[(i - 1) * number + (j - 1)] = n;
            n *= i;
        }
    }
    ans[10 * number] = 0;
    return 0;
}


int HandlerOptA(int number, long long ans[]) {
    if (number <= 0) return 1;
    ans[1] = 0;
    long long res = number * (number + 1) * 0.5;
    ans[0] = res;
    return 0;

}

int HandlerOptF(int number, long long ans[]) {
    ans[1] = 0;
    long long factorial = 1;
    if (number < 0) return 1;
    if (number > 20){
        return 106;
    }
    for (int i = 1; i <= number; i++)
    {
        factorial *= i;
    }
    ans[0] = factorial;
    return 0;
}

int main(int argc, char** argv) {
    kOpts opt = OPT_H;
    int procceed_number = 0;

    int (*handlers[6])(int, long long[]) = {
            HandlerOptH,
            HandlerOptP,
            HandlerOptS,
            HandlerOptE,
            HandlerOptA,
            HandlerOptF
    };
    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("%s", "Incorrect option");
        return 1;
    }

    long long ans[100];

    int key = handlers[opt](procceed_number, ans);

    switch (key) {
        case 1:
            printf("Bad number");
            return 1;
        case 101:
            printf("Too big number");
            return 1;
        case 102:
            printf("1 is neither composite nor simple");
            return 1;
        case 106:
            printf("Too big to calculate factorial");
            return 1;
        default:
            switch (opt){
                case OPT_H:
                    for(int i = 0; ans[i]; i++) printf("%lld ", ans[i]);
                    break;
                case OPT_P:
                    if (ans[0]) printf("%d is simple\n", procceed_number);
                    else printf("%d is not simple\n", procceed_number);
                    break;
                case OPT_S:
                    for(int i = 0; ans[i]; i++){
                        printf("%c ",(char)ans[i]);
                    }
                    break;
                case OPT_E:
                    for (int i = 0; ans[i]; i++){
                        printf("%12lld", ans[i]);
                        if ((i + 1) % procceed_number == 0) {
                            printf("\n");
                        }
                    }
                    break;
                default:
                    printf("%lld", ans[0]);
                    break;
            }
    }
//    if (opt == OPT_F && procceed_number > 20){
//        printf("too big number to calculate factorial\n");
//    }
//    else{
//        printf(text[opt], handlers[opt](procceed_number));
//    }

    return 0;
}
