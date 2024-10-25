#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include <limits.h>
#include <math.h>
//#include <unistd.h>

typedef enum kOpts {
    OPT_D,
    OPT_I,
    OPT_S,
    OPT_A
} kOpts;

typedef enum Errors {
    Incorrect_input = 2,
    Equal_dest,
    Unable_to_open_file
} Errors;

void hex(int x, char* result, int size);

int HandlerOptD(char* in, char* out);

int HandlerOptI(char* in, char* out);

int HandlerOptS(char* in, char* out);

int HandlerOptA(char* in, char* out);

int check_file_names(const char *file1, const char *file2);
void hex(int x, char* result, int size) {
    char letters[16] = "0123456789ABCDEF";
    int i = size - 1;

    while (x > 0 && i >= 0) {
        result[i--] = letters[x % 16];
        x /= 16;
    }
}

int HandlerOptD(char* in, char* out) {
    FILE* f1 = fopen(in, "r");
    FILE* f2 = fopen(out, "w");

    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return Unable_to_open_file;
    }
    int ch;
    while ((ch = fgetc(f1)) != EOF) {
        if (ch > 0 && !(ch >= '0' && ch <= '9'))
            fputc(ch, f2);
    }
    fclose(f1);
    fclose(f2);
    return 0;
}

int HandlerOptI(char* in, char* out) {
    FILE* f1 = fopen(in, "r");
    FILE* f2 = fopen(out, "w");
    if (!f1 || !f2) {
        if (f1) {
            fclose(f1);
        }
        if (f2) {
            fclose(f2);
        }
        return Unable_to_open_file;
    }

    int ch, english_letters = 0, line_counter = 0, flag = 0;
    while ((ch = fgetc(f1)) != EOF) {
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            english_letters++;
            flag = 1;
        }
        else if (ch == '\n') {
            flag = 0;
            line_counter++;
            fprintf(f2, "String %d contains %d eng letters\n", line_counter, english_letters);
            english_letters = 0;
        }
    }

    if (flag)
        fprintf(f2, "String %d contains %d eng letters\n", line_counter + 1, english_letters);


    fclose(f1);
    fclose(f2);
    return 0;
}

int HandlerOptS(char* in, char* out) {
    FILE* f1 = fopen(in, "r");
    FILE* f2 = fopen(out, "w");

    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return Unable_to_open_file;
    }

    int ch, symbols = 0, line_counter = 0;
    while ((ch = fgetc(f1)) != EOF) {
        if (ch > 0 && !(((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) || (ch >= '0' && ch <= '9') || ch == ' ') && ch != '\n') {
            symbols++;
        } else if (ch == '\n') {
            line_counter++;
            fprintf(f2, "String %d contains %d no english, no arabica number, no gap symbols\n", line_counter, symbols);
            symbols = 0;
        }

    }
    fprintf(f2, "String %d contains %d no english, no arabica number, no gap symbols\n", line_counter + 1, symbols);


    fclose(f1);
    fclose(f2);
    return 0;
}

int HandlerOptA(char* in, char* out) {
    FILE* f1 = fopen(in, "r");
    FILE* f2 = fopen(out, "w");

    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return Unable_to_open_file;
    }
    int ch;
    while ((ch = fgetc(f1)) != EOF) {

        if (ch > 0 && !(ch >= '0' && ch <= '9')) {
            int size = (int)(log2(ch) * 0.25) + 1;
            char result[size];
            hex(ch, result, size);
            fprintf(f2, "%s", result);
        } else if (ch > 0) fputc(ch, f2);
    }
    fclose(f1);
    fclose(f2);
    return 0;
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



int GetOpts(int argc, char** argv, kOpts* opt, char* in_path, char* out_path) {
    if (argc < 2 || argc > 4) {
        return Incorrect_input;
    }

    const char* proceeding_option = argv[1];
    if (proceeding_option[0] == '/' || proceeding_option[0] == '-') {
        char flag = proceeding_option[1];
        strcpy(in_path, argv[2]);

        if (flag == 'n' && argc == 4) {
            flag = proceeding_option[2];
            strcpy(out_path, argv[3]);
        } else if (flag != 'n' && argc == 3) {
            strcat(out_path, in_path);
        } else {
            return Incorrect_input;
        }

        switch (flag) {
            case 'd':
                *opt = OPT_D;
                break;
            case 'i':
                *opt = OPT_I;
                break;
            case 's':
                *opt = OPT_S;
                break;
            case 'a':
                *opt = OPT_A;
                break;
            default:

                return Incorrect_input;
        }
    } else {

        return Incorrect_input;
    }

    if (check_file_names(in_path, out_path)) {

        return Equal_dest;
    }

    return 0;
}
int main(int argc, char** argv) {
    kOpts opt = 1;
    char out_path[PATH_MAX] = "out_", in_path[PATH_MAX];

    int result = GetOpts(argc, argv, &opt, in_path, out_path);
    if (result == Incorrect_input) {
        printf("Incorrect input");
        return 2;
    }else if (result == Equal_dest) {
        printf("Equal_dest");
        return 3;
    }

    int (*handlers[4])(char*, char*) = {HandlerOptD,
                                        HandlerOptI,
                                        HandlerOptS,
                                        HandlerOptA};
    result = handlers[opt](in_path, out_path);
    switch (result) {
        case Incorrect_input:
            printf("Incorrect input");
            break;
        case Equal_dest:
            printf("Equal destination");
            break;
        case Unable_to_open_file:
            printf("Unable to open file");
            break;
        default:
            return result;
    }
}