#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int add(char **array, int *size, int const i, char const element) {
    if (i >= *size) {
        *size *= 2;
        char *temp = (char *) realloc(*array, *size * sizeof(char));
        if (temp == NULL) {
            return 1;
        }
        *array = temp;

    }
    (*array)[i] = element;
    return 0;
}



int Convert_to_10(const char *number, int radix, long long* a) {
    long long ans = 0;
    int d;
    int sign = 1;
    for (int i = 0; number[i]; i++) {
        if (number[i] == '-') {
            sign = -1;
            continue;
        }
        d = ('0' <= number[i] && '9' >= number[i]) ? number[i] - '0' : number[i] % 32 + 9;
        long long t = ans;
        ans = ans * radix;
        if (ans / radix != t) return 1;
        if (sign == 1) if (ans > LONG_MAX - d) return 1;
        if (sign == -1) if (ans < LONG_MIN + d) return 1;
        ans += sign * d;
    }
    *a = ans;
    return 0;
}

int number_into_char(long long const element, char **ans, int *index, int *size) {
    char temp[32];
    long long n = element;
    if (n < 0) {
        if (add(ans, size, *index, '-')) return 1;
        *index += 1;
        n = -n;
    }
    int i = 0;
    do {
        temp[i++] = n % 10 + '0';
        n /= 10;
    } while (n > 0);
    for (int j = 0; j < i; j++) {
        if (add(ans, size, *index, temp[i - j - 1])) return 1;
        *index += 1;
    }
    return 0;
}

int made_ans(char *number, char **ans, int *size_ans, int *index) {
    int radix = 2;
    int flag = 0;
    short sign = (number[0] == '-') ? -1 : 1;
    if (sign == -1) {
        if (add(ans, size_ans, *index, '-')) return 1;
        *index += 1;
    }
    for (int i = (1 - sign) / 2; number[i]; i++) {
        int d = ('0' <= number[i] && '9' >= number[i]) ? number[i] - '0' + 1 : number[i] % 32 + 10;
        radix = (d > radix) ? d : radix;
        if (number[i] != '0' || flag) {
            flag = 1;
            if (add(ans, size_ans, *index, number[i])) return 1;
            *index += 1;
        }
    }
    if (!flag) {
        if (sign == 1) { if (add(ans, size_ans, *index++, '0')) return 1; }
        else return 1;
    }

    if (add(ans, size_ans, *index, ' ')) return 1;
    *index += 1;
    if (number_into_char(radix, ans, index, size_ans)) return 1;
    if (add(ans, size_ans, *index, ' ')) return 1;
    *index += 1;
    long long an;
    if (Convert_to_10(number, radix, &an)) return 1;
    if (number_into_char(an, ans, index, size_ans)) return 1;
    if(add(ans, size_ans, *index, '\n')) return 1;
    *index += 1;
    return 0;
}

int GetOpts(int argc, char **argv, char *input, char *output, int *size_i, int *size_o) {
    if (argc != 3) {
        return 1;
    }
    int j;
    for (j = 0; argv[1][j]; j++) {
        if ('\\' == argv[1][j] || '/' == argv[1][j]) {
            if (add(&input, size_i, j, '/')) return 1;
        } else if (add(&input, size_i, j, argv[1][j])) return 1;
    }
    if (add(&input, size_i, j, '\0')) return 1;
    int i;
    for (i = 0; argv[2][i]; i++) {
        if (argv[2][i] == '\\') { if (add(&output, size_o, i, '/')) return 1; }
        else if (add(&output, size_o, i, argv[2][i])) return 1;
    }
    if (add(&output, size_o, i, '\0')) return 1;
    return 0;
}

int Handler(FILE *in_put, char **ans, int *size) {
    int size_n = 10;
    char *number = (char *) malloc(size_n * sizeof(char));
    if (number == NULL) return 1;
    number[0] = '\0';
    char c;
    int i = 0;
    int index = 0;
    while ((c = (char) fgetc(in_put)) != EOF) {
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (!i && c == '-')) {
            if (add(&number, &size_n, i++, c)) {
                free(number);
                return 1;
            }
        } else if (c == ' ' || c == '\n' || c == '\t') {
            if (add(&number, size, i, '\0')) {
                free(number);
                return 1;
            }
            if (number[0] == '-' && !number[1]) {
                free(number);
                return 1;
            }
            if (number[0]) {
                if (made_ans(number, ans, size, &index)) {
                    free(number);
                    return 1;
                }
                number[0] = '\0';
            }
            i = 0;
        } else {
            free(number);
            return 1;
        }
    }
    if (i) {
        if (add(&number, size, i, '\0')) {
            free(number);
            return 1;
        }
        if (number[0] == '-' && !number[1]) {
            free(number);
            return 1;
        }
        if (number[0]) {
            if (made_ans(number, ans, size, &index)) {
                free(number);
                return 1;
            }

        }
    }
    if (add(ans, size, index, '\0')) {
        free(number);
        return 1;
    }
    free(number);
    return 0;

}
int main(int argc, char **argv) {

    int size_i = 32;
    int size_o = 32;
    char *in_put_file = (char *) malloc(size_i * sizeof(char));
    if (!in_put_file) {
        printf("Memory allocation error");
        return 1;
    }
    char *out_put_file = (char *) malloc(size_o * sizeof(char));
    if (!out_put_file) {
        printf("Memory allocation error");
        free(in_put_file);
        return 1;
    }
    if (GetOpts(argc, argv, in_put_file, out_put_file, &size_i, &size_o)) {
        printf("%s", "Incorrect option");
        free(in_put_file);
        free(out_put_file);
        return 1;
    }
    FILE *in_put = fopen(in_put_file, "r");
    if (in_put == NULL) {
        printf("%s", "Incorrect file in put file");
        free(in_put_file);
        free(out_put_file);
        return 1;
    }
    int size = 10;
    char *answer = (char *) malloc((size + 1) * sizeof(char));
    if (answer == NULL)
    {
        fclose(in_put);
        free(in_put_file);
        free(out_put_file);
        printf("Memory allocation error");
        return 1;
    }

    if (Handler(in_put, &answer, &size)) {
        fclose(in_put);
        free(answer);
        free(in_put_file);
        free(out_put_file);
        printf("%s", "Not relevant data");
        return 1;
    }
    fclose(in_put);

    FILE *out_put = fopen(out_put_file, "w");
    if (out_put == NULL) {
        printf("%s", "Incorrect file out put file");
        free(answer);
        free(in_put_file);
        free(out_put_file);
        return 1;
    }
    fprintf(out_put,"%s", answer);

    free(answer);
    free(in_put_file);
    free(out_put_file);
    fclose(out_put);
    return 0;
}