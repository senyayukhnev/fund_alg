#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "limits.h"
typedef enum errorCodes{
    Not_enough_arg,
    Too_many_arg,
    Unknown_flag,
    Not_a_flag,
    Unable_to_open_file,
    Invalid_int,
    Memory_allocate_err,
    Normal
} errorCodes;
typedef enum Options{
    FLAG_L,
    FLAG_R,
    FLAG_U,
    FLAG_N,
    FLAG_C,
} Options;

errorCodes convert_str_to_int(char* num_str, unsigned int* out_num){
    unsigned int parsed_num = 0;
    for (int j = 0; num_str[j]; j++){
        char character = num_str[j];
        if (character >= '0' && character <= '9'){
            if (parsed_num > UINT_MAX / 10 - character + '0'){
                return Invalid_int;
            }
            parsed_num *= 10;
            parsed_num += character - '0';
        } else{
            return Invalid_int;
        }
    }

    *out_num = parsed_num;
    return Normal;
}

errorCodes recognize_flag(const char* flag_str, Options* selected_flag){
    if (flag_str[0] == '-' || flag_str[0] == '/'){
        switch (flag_str[1]) {
            case 'l':
                *selected_flag = FLAG_L;
                break;
            case 'r':
                *selected_flag = FLAG_R;
                break;
            case 'u':
                *selected_flag = FLAG_U;
                break;
            case 'n':
                *selected_flag = FLAG_N;
                break;
            case 'c':
                *selected_flag = FLAG_C;
                break;
            default:
                return Unknown_flag;
        }
        if (flag_str[2]){
            return Unknown_flag;
        }
    } else {
        return Not_a_flag;
    }
    return Normal;
}

errorCodes parse_arguments(int arg_count, char **arg_values, Options *selected_flag, unsigned int* number, char*** str_list){
    if (arg_count < 3) return Not_enough_arg;

    errorCodes flag_status = recognize_flag(arg_values[1], selected_flag);
    if (flag_status != Normal) return flag_status;

    switch (*selected_flag) {
        case FLAG_C:
            if (arg_count < 4) return Not_enough_arg;
            *str_list = malloc(sizeof(char*) * 2 + sizeof(char*) * (arg_count - 4));
            if (*str_list == NULL) return Memory_allocate_err;
            (*str_list)[0] = arg_values[2];

            errorCodes num_status = convert_str_to_int(arg_values[3], number);
            if (num_status != Normal) return num_status;

            if (arg_count < 5) break;
            for (int k = 1; k < arg_count - 3; k++){
                (*str_list)[k] = arg_values[k + 3];
            }
            (*str_list)[arg_count - 3] = NULL;
            break;
        default:
            if (arg_count > 3) return Too_many_arg;
            *str_list = malloc(sizeof(char*) * 2);
            if (*str_list == NULL) return Memory_allocate_err;
            (*str_list)[0] = arg_values[2];
            (*str_list)[1] = NULL;
            break;
    }
    return Normal;
}

int calc_length(const char* text){
    int length = 0;
    for (int k = 0; text[k]; k++) length++;
    return length;
}

int handleFlagL(const char* text, int *len_output){
    *len_output = calc_length(text);
    return 0;
}

int handleFlagR(const char* text, char** reversed_text){
    int length = calc_length(text);
    for (int j = 0; j < length; j++) (*reversed_text)[j] = text[length - j - 1];
    (*reversed_text)[length] = '\0';
    return 0;
}

int handleFlagU(const char* text, char** upper_text){
    for (int k = 0; text[k]; k++) (*upper_text)[k] = (k % 2 == 0) ? text[k] : toupper(text[k]);
    return 0;
}

int handleFlagN(const char* text, char** modified_text){
    int length = calc_length(text);
    int digits = 0, letters = 0, others = 0;
    for (int j = 0; j < length; j++){
        char character = text[j];
        if (character >= '0' && character <= '9') (*modified_text)[digits++] = character;
    }
    letters += digits;
    for (int j = 0; j < length; j++){
        char character = text[j];
        if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z')) (*modified_text)[letters++] = character;
    }
    others += letters;
    for (int j = 0; j < length; j++){
        char character = text[j];
        if (!isalnum(character)) (*modified_text)[others++] = character;
    }
    (*modified_text)[length] = '\0';
    return 0;
}

int handleFlagC(char** str_arr, int seed, char **concat_result){
    srand(seed);
    int total_length = 0;
    int count = 0;
    for (int i = 0; str_arr[i]; i++) {
        total_length += calc_length(str_arr[i]);
        count += 1;
    }

    for (int i = count - 1; i > 0; i--) {
        int swap_idx = rand() % (i + 1);
        char* temp = str_arr[i];
        str_arr[i] = str_arr[swap_idx];
        str_arr[swap_idx] = temp;
    }

    char *result = malloc((total_length + 1) * sizeof(char));
    if (result == NULL) return Memory_allocate_err;
    result[total_length] = '\0';

    int pos = 0;
    for (int i = 0; str_arr[i]; i++) {
        for (int j = 0; str_arr[i][j]; j++) {
            result[pos++] = str_arr[i][j];
        }
    }

    *concat_result = result;
    return Normal;
}

int main(int arg_count, char** arg_values){
    Options selected_flag;
    unsigned int number;
    char** str_list;

    const errorCodes parse_status = parse_arguments(arg_count, arg_values, &selected_flag, &number, &str_list);
    if (parse_status != Normal){
        switch (parse_status){
            case Not_enough_arg:
                printf("Insufficient arguments provided for the selected flag.\n");
                break;
            case Too_many_arg:
                printf("Too many arguments provided for the selected flag.\n");
                break;
            case Invalid_int:
                printf("Error reading integer value. The fourth argument must be a positive number.\n");
                break;
            case Unknown_flag:
                printf("Error. Unrecognized flag.\n");
                break;
            case Not_a_flag:
                printf("Error. First argument must be a valid flag.\n");
                break;
            case Memory_allocate_err:
                printf("Error allocating memory.\n");
                break;
        }
        return 1;
    }

    switch (selected_flag){
        case FLAG_L:
            int len_output = 0;
            handleFlagL(str_list[0], &len_output);
            printf("String length: %d\n", len_output);
            break;
        case FLAG_R:
            char* reversed_text = (char*)malloc(sizeof(char) * (calc_length(str_list[0]) + 1));
            handleFlagR(str_list[0], &reversed_text);
            printf("Reversed string: %s\n", reversed_text);
            free(reversed_text);
            break;
        case FLAG_U:
            char* upper_text = (char*)malloc(sizeof(char) * (calc_length(str_list[0]) + 1));
            handleFlagU(str_list[0], &upper_text);
            printf("Uppercased string: %s\n", upper_text);
            free(upper_text);
            break;
        case FLAG_N:
            char* modified_text = (char*)malloc(sizeof(char) * (calc_length(str_list[0]) + 1));
            handleFlagN(str_list[0], &modified_text);
            printf("Modified string: %s\n", modified_text);
            free(modified_text);
            break;
        case FLAG_C:
            char *concat_result;
            errorCodes concat_status = handleFlagC(str_list, number, &concat_result);
            if (concat_status != Normal) {
                printf("Error during concatenation.\n");
                return 1;
            }
            printf("Concatenated string: %s\n", concat_result);
            free(concat_result);
            break;
        default:
            break;
    }
    free(str_list);
    return 0;
}
