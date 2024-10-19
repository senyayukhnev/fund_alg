#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
typedef enum errorCodes{
    Not_enough_arg,
    Too_many_arg,
    Unknown_flag,
    Not_a_flag,
    Unable_to_open_file,
    Normal
} errorCodes;

typedef enum kOpts{
    OPT_R,
    OPT_A
} kOpts;

typedef enum errHandlersCodes{
    Write_file_error = 1,
    Malloc_error = 2
} errHandlersCodes;

errorCodes parseFlag(char* proceeding_string, kOpts* flag){
    if (proceeding_string[0] == '-' || proceeding_string[0] == '/'){
        switch (proceeding_string[1])
        {
            case 'r':
                *flag = OPT_R;
                break;
            case 'a':
                *flag = OPT_A;
                break;
            default:
                return Unknown_flag;
        }
        if (proceeding_string[2]){
            return Unknown_flag;
        }
    } else{
        return Not_a_flag;
    }

    return Normal;
}
int check_file_names(const char *file1, const char *file2) {
    const char * name1 = strrchr(file1,'\\');
    const char * name2 = strrchr(file2,'\\');
    if (name1 != NULL) {
        name1++;
    } else {
        name1 = file1;
    }
    if (name2 != NULL) {
        name2++;
    } else {
        name2 = file2;
    }

    if (strcmp(name1, name2) == 0) {
        return 1;
    }
    return 0;
}

errorCodes is_same_file(char *a, char *b) {
    errorCodes res = Normal;
    if (check_file_names(a, b)) {
        res = Unable_to_open_file;
    }
    return res;
}


errorCodes getArgs(int argc, char** argv, kOpts *flag, FILE *files[3]){
    if(argc < 4) return Not_enough_arg;
    else if(argc > 5) return Too_many_arg;

    char* proceeding_flag = argv[1];
    errorCodes flag_err = parseFlag(proceeding_flag, flag);
    if(flag_err != Normal) return flag_err;

    switch (*flag){
        case OPT_R:
            if(argc < 5) return Not_enough_arg;

            if((is_same_file(argv[2], argv[4]) != Normal) || (is_same_file(argv[3], argv[4]) != Normal)){
                return Unable_to_open_file;
            }

            FILE *in1, *in2, *out;
            in1 = fopen(argv[2], "r");
            in2 = fopen(argv[3], "r");
            out = fopen(argv[4], "w");
            if (in1 == NULL || in2 == NULL || out == NULL){
                return Unable_to_open_file;
            }
            files[0] = in1;
            files[1] = in2;
            files[2] = out;

            break;
        case OPT_A:
            if(argc > 4) return Too_many_arg;

            if(is_same_file(argv[2], argv[3]) != Normal){
                return Unable_to_open_file;
            }

            FILE *in_file, *output_file;
            in_file = fopen(argv[2], "r");
            output_file = fopen(argv[3], "w");
            if (in_file == NULL || output_file == NULL){
                return Unable_to_open_file;
            }
            files[0] = in_file;
            files[1] = output_file;

            break;
    }
    return Normal;
}

void next_lexem(int* c, FILE* file){
    while((*c = fgetc(file)) != EOF){
        if(!(*c == ' ' || *c == '\t' || *c == '\n')) break;
    }
    fseek(file, -1, SEEK_CUR);
}

int handlerOptR(FILE *files[3]){
    FILE *in1 = files[0];
    FILE *in2 = files[1];
    FILE *out = files[2];
    int ch1, ch2;
    int index_lexem = 1;

    next_lexem(&ch1, in1);
    next_lexem(&ch2, in2);

    while (ch1 != EOF && ch2 != EOF){
        if (index_lexem % 2 == 1){
            while((ch1 = fgetc(in1)) != EOF){
                if((ch1 == ' ' || ch1 == '\n' || ch1 == '\t')) break;
                if(fputc(ch1, out) == EOF) return 1;
            }
            if(fputc(' ', out) == EOF) return 1;
            next_lexem(&ch1, in1);
            index_lexem++;
        } else{
            while((ch2 = fgetc(in2)) != EOF){
                if((ch2 == ' ' || ch2 == '\n' || ch2 == '\t')) break;
                if(fputc(ch2, out) == EOF) return 1;
            }
            if(fputc(' ', out) == EOF) return 1;
            next_lexem(&ch2, in2);
            index_lexem++;
        }
    }

    int ch = (ch1 != EOF) ? ch1 : ch2;
    FILE *in_file = (ch1 != EOF) ? in1 : in2;

    while((ch = fgetc(in_file)) != EOF){
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            next_lexem(&ch, in_file);
            if(ch == EOF) break;
            if(fputc(' ', out) == EOF) return 1;
            continue;
        }
        if(fputc(ch, out) == EOF) return 1;
    }

    return 0;
}



char* gorner(int n, int base){
    int length = (int)(log(n)/log(base)) + 1;
    char* result = (char*)malloc(sizeof(char) * (length + 1));
    if(result == NULL) return NULL;
    result[length] = '\0';
    if(n < base){
        if(n >= 10) result[0] = (char)((n - 10) + 'a');
        else result[0] = (char)(n + '0');
    }
    while(n >= base){
        int div_remain = n % base;
        if(div_remain < 10){
            result[--length] = (char)('0' + div_remain);
        } else{
            result[--length] = (char)('a' + (div_remain - 10));
        }
        n /= base;
        if(n < base){
            if(n >= 10) result[0] = (char)((n - 10) + 'a');
            else result[0] = (char)(n + '0');
        }
    }
    return result;
}

int handlerOptA(FILE *files[3]){
    FILE *in_file = files[0];
    FILE *output = files[1];
    int ch;
    int index_lexem = 1;
    next_lexem(&ch, in_file);
    while (1){
        ch = fgetc(in_file);
        if(ch == EOF) break;
        if(index_lexem % 10 == 0){
            while (ch != EOF && ch != ' ' && ch != '\n' && ch != '\t') {
                char* converted = gorner(ch, 4);
                if (converted == NULL) return Malloc_error;
                fprintf(output, "%s", converted);
                free(converted);
                ch = fgetc(in_file);
            }
            next_lexem(&ch, in_file);
            index_lexem++;
        }
        else if(index_lexem % 2 == 0 && index_lexem % 10 != 0){
            while (ch != EOF && ch != ' ' && ch != '\n' && ch != '\t') {
                char tmp = (char)tolower(ch);
                fputc(tmp, output);
                ch = fgetc(in_file);
            }
            next_lexem(&ch, in_file);
            index_lexem++;
        }
        else if(index_lexem % 5 == 0 && index_lexem % 10 != 0){
            while (ch != EOF && ch != ' ' && ch != '\n' && ch != '\t') {
                char* converted = gorner(ch, 8);
                if (converted == NULL) return Malloc_error;
                fprintf(output, "%s", converted);
                free(converted);
                ch = fgetc(in_file);
            }
            next_lexem(&ch, in_file);
            index_lexem++;
        }
        else{
            if(ch == ' ' || ch == '\t' || ch == '\n') next_lexem(&ch, in_file);
            else{
                while (ch != EOF){
                    if (ch == ' ' || ch == '\t' || ch == '\n') break;
                    fputc(ch, output);
                    ch = fgetc(in_file);
                }
                if (ch == EOF) break;
                next_lexem(&ch, in_file);
            }
            index_lexem++;
        }
        if (ch == EOF) break;
        fputc(' ', output);
    }
    return 0;
}


int main(int argc, char** argv){
    kOpts flag;
    FILE *files[3] = {NULL, NULL, NULL};
    int (*handlers[2])(FILE *files[3]) = {
            handlerOptR,
            handlerOptA
    };

    errorCodes err_status = getArgs(argc, argv, &flag, files);
    if(err_status != Normal){ // handling errors from cli input
        switch (err_status)
        {
            case Not_a_flag:
                printf("%s \n", "Not a flag");
                break;
            case Unknown_flag:
                printf("%s \n", "Unknown flag");
            case Not_enough_arg:
                printf("%s \n", "Not enough number of argumnets.");
                break;
            case Too_many_arg:
                printf("%s \n", "Too many argumnets.");
                break;
            case Unable_to_open_file:
                printf("Cannot open file \n");
                break;
        }
        return 1;
    }

    switch (handlers[flag](files)){
        case Malloc_error:
            printf("malloc error\n");
            break;
        case Write_file_error:
            printf("Error writing to file occured\n");
            break;
        default:
            printf("Output file has been successfully changed\n");
            break;
    }

    for (int i = 0; i < 3; i++) {
        if (files[i]) {
            fclose(files[i]);
        }
    }
    return 0;
}