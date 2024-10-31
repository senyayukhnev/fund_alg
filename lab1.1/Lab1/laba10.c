#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef enum {
    Normal = 0,
    ERR_STDIN_FALL = 1,
    ERR_INCORRECT_BASE = 2,
    ERR_INVALID_INPUT = 3,
    ERR_OVERFLOW = 4
} status;

long long int convert_to_decimal(const char* num_str, int base);
void convert_from_decimal(long long int value, int base, char* result);

long long int convert_to_decimal(const char* num_str, int base) {
    long long int value = 0;
    int len = strlen(num_str);
    int start_index = 0;

    if (num_str[0] == '-') {
        start_index = 1;
    }

    for (int i = start_index; i < len; i++) {
        int digit;
        if (num_str[i] >= '0' && num_str[i] <= '9') {
            digit = num_str[i] - '0';
        } else if (num_str[i] >= 'A' && num_str[i] <= 'Z') {
            digit = num_str[i] - 'A' + 10;
        } else if (num_str[i] >= 'a' && num_str[i] <= 'z') {
            digit = num_str[i] - 'a' + 10;
        } else {
            return ERR_INVALID_INPUT;
        }

        if (digit >= base) {
            return ERR_INCORRECT_BASE;
        }

        if (value > (LLONG_MAX - digit) / base) {
            return ERR_OVERFLOW;
        }

        value = value * base + digit;
    }

    return (num_str[0] == '-') ? -value : value;
}

void convert_from_decimal(long long int value, int base, char* result) {
    char buffer[BUFSIZ];
    int index = 0;

    if (value == 0) {
        strcpy(result, "0");
        return;
    }

    long long int abs_value = llabs(value);
    while (abs_value > 0) {
        int digit = abs_value % base;
        if (digit < 10) {
            buffer[index++] = '0' + digit;
        } else {
            buffer[index++] = 'A' + (digit - 10);
        }
        abs_value /= base;
    }

    for (int i = index - 1; i >= 0; i--) {
        result[index - 1 - i] = buffer[i];
    }
    result[index] = '\0';

    if (value < 0) {
        char temp[BUFSIZ];
        strcpy(temp, result);
        sprintf(result, "-%s", temp);
    }
}

int main() {
    int base;
    char input[BUFSIZ];
    long long int max_value = 0;
    char max_number[BUFSIZ] = "";
    int found_valid_number = 0;

    printf("Enter the base of the number system (from 2 to 36, or 'Stop' to exit): ");
    fgets(input, sizeof(input), stdin);


    input[strcspn(input, "\n")] = 0;

    if (strcmp(input, "Stop") == 0) {
        printf("Exit on command 'Stop'.\n");
        return Normal;
    }


    base = atoi(input);

    if (base < 2 || base > 36) {
        return ERR_INCORRECT_BASE;
    }

    printf("Enter numbers in base %d (type 'Stop' to stop):\n", base);

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return ERR_STDIN_FALL;
        }

        input[strcspn(input, "\n")] = 0;

        char* token = strtok(input, " \t");

        while (token != NULL) {
            if (strcmp(token, "Stop") == 0) {
                if (found_valid_number) {
                    printf("Maximum number by module: %lld\n", max_value);

                    char result[BUFSIZ];

                    printf("Representation in base 9 notation: ");
                    convert_from_decimal(max_value, 9, result);
                    printf("%s\n", result);

                    printf("Representation in base 18 notation: ");
                    convert_from_decimal(max_value, 18, result);
                    printf("%s\n", result);

                    printf("Representation in base 27 notation: ");
                    convert_from_decimal(max_value, 27, result);
                    printf("%s\n", result);

                    printf("Representation in base 36 notation: ");
                    convert_from_decimal(max_value, 36, result);
                    printf("%s\n", result);
                } else {
                    printf("No valid numbers entered.\n");
                }
                return Normal;
            }


            long long int value = convert_to_decimal(token, base);

            if (value == ERR_INVALID_INPUT) {
                printf("Error: Invalid number '%s' for base %d.\n", token, base);
                return ERR_INVALID_INPUT;
            } else if (value == ERR_INCORRECT_BASE) {
                printf("Error: Invalid base for number '%s'.\n", token);
                return ERR_INCORRECT_BASE;
            } else if (value == ERR_OVERFLOW) {
                printf("Error: The number '%s' was too large and caused an overflow.\n", token);
                return ERR_OVERFLOW;
            } else {
                found_valid_number = 1;
                if (llabs(value) > llabs(max_value)) {
                    max_value = value;
                    strcpy(max_number, token);
                }
            }
            token = strtok(NULL, " \t");
        }
    }
}