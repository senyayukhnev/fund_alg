#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "limits.h"
#include "float.h"

typedef struct Employee{
    long long id;
    char name[512];
    char surname[512];
    double salary;
}Employee;

typedef enum{
    Normal,
    Not_enough_arg,
    Too_many_arg,
    Incorrect_input,
    Unknown_flag,
    File_not_found,
    Equal_path,
    Malloc_err,
    Overflow,
    Incorrect_salary,
    Incorrect_name,
    Incorrect_surname,
    Incorrect_id,
    Write_file_err,
    Data_emp_err

}Err;

typedef enum{
    OPT_A,
    OPT_D
}kOpts;

Err check_file_names(const char *file1, const char *file2) {
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
        return Equal_path;
    }
    return Normal;
}

int GetOpts(int argc, char** argv, kOpts* opt, char* in_path, char* out_path) {
    if (argc > 4) {
        return Too_many_arg;
    }
    if (argc < 4) {
        return Not_enough_arg;
    }

    const char* proceeding_option = argv[2];
    if (proceeding_option[0] == '/' || proceeding_option[0] == '-') {
        char flag = proceeding_option[1];
        strcpy(in_path, argv[1]);
        strcpy(out_path, argv[3]);


        switch (flag) {
            case 'd':
                *opt = OPT_D;
                break;
            case 'a':
                *opt = OPT_A;
                break;
            default:
                return Unknown_flag;
        }
        if (proceeding_option[2]){
            return Unknown_flag;
        }
    } else {

        return Incorrect_input;
    }


if (check_file_names(in_path, out_path)) {
        return Equal_path;
    }

    return Normal;
}
int comparator_a(const void *a, const void *b) {
    const Employee *emp1 = (const Employee *)a;
    const Employee *emp2 = (const Employee *)b;

    if (emp1->salary > emp2->salary) return 1;
    if (emp1->salary < emp2->salary) return -1;

    int surname_cmp = strcmp(emp1->surname, emp2->surname);
    if (surname_cmp != 0) return surname_cmp;

    int name_cmp = strcmp(emp1->name, emp2->name);
    if (name_cmp != 0) return name_cmp;

    return emp1->id - emp2->id;
}

int comparator_d(const void *a, const void *b) {
    const Employee *emp1 = (const Employee *)a;
    const Employee *emp2 = (const Employee *)b;

    if (emp1->salary > emp2->salary) return -1;
    if (emp1->salary < emp2->salary) return 1;

    int surname_cmp = strcmp(emp2->surname, emp1->surname);
    if (surname_cmp != 0) return surname_cmp;

    int name_cmp = strcmp(emp2->name, emp1->name);
    if (name_cmp != 0) return name_cmp;

    return emp2->id - emp1->id;
}

void next_lexem(int* c, FILE* file) {
    while ((*c = fgetc(file)) != EOF) {
        if (!(*c == ' ' || *c == '\t' || *c == '\n')) break;
    }
}
int get_Emp(FILE *input, Employee* employee, int* ch) {
    if (!input) return File_not_found;
    if (feof(input)) return EOF;

    employee->id = 0;
    employee->salary = 0.0;
    employee->name[0] = '\0';
    employee->surname[0] = '\0';

    int count_of_lex = 1;
    while (count_of_lex != 5) {
        if (*ch == EOF) break;

        if (count_of_lex == 1){  // Чтение id
            while (*ch != ' ' && *ch != '\t' && *ch != '\n') {
                if (*ch >= '0' && *ch <= '9') {
                    if (employee->id > (LLONG_MAX - (*ch - '0')) / 10) { // Проверка переполнения
                        while (*ch != ' ' && *ch != EOF) *ch = fgetc(input);
                        *ch = fgetc(input);
                        return Incorrect_id;
                    }
                    employee->id = employee->id * 10 + (*ch - '0');
                } else {
                    while (*ch != ' ' && *ch != EOF) *ch = fgetc(input);
                    *ch = fgetc(input);
                    return Incorrect_id;
                }
                *ch = fgetc(input);
            }
            if (employee->id < 0) return Incorrect_id; // Проверка на отрицательность и нулевое значение
            next_lexem(ch, input);
            count_of_lex++;
        }

        if (count_of_lex == 2){
            while (*ch != ' ' && *ch != '\t' && *ch != '\n') {
                if ((*ch >= 'A' && *ch <= 'Z') || (*ch >= 'a' && *ch <= 'z')) {
                    unsigned int len = strlen(employee->name);
                    if (len < sizeof(employee->name) - 1) {
                        employee->name[len] = (char) *ch;
                        employee->name[len + 1] = '\0';
                    }
                }else{
                    while (*ch != ' ' && *ch != EOF) *ch = fgetc(input);
                    *ch = fgetc(input);
                    return Incorrect_name;
                }
                *ch = fgetc(input);
            }
            next_lexem(ch, input);
            count_of_lex++; // 3
        }
        if (count_of_lex == 3){

            while (*ch != ' ' && *ch != '\t' && *ch != '\n') {
                if ((*ch >= 'A' && *ch <= 'Z') || (*ch >= 'a' && *ch <= 'z')) {
                    unsigned int len = strlen(employee->surname);
                    if (len < sizeof(employee->surname) - 1) {
                        employee->surname[len] = (char) *ch;
                        employee->surname[len + 1] = '\0';
                    }
                }else{
                    while (*ch != ' ' && *ch != EOF) *ch = fgetc(input);
                    *ch = fgetc(input);
                    return Incorrect_surname;
                }
                *ch = fgetc(input);
            }
            next_lexem(ch, input);
            count_of_lex++; // 4
        }
        if (count_of_lex == 4) {  // Чтение salary
            int f = 0, after_dot = 0;
            while (*ch != ' ' && *ch != '\t' && *ch != '\n' && *ch != EOF) {
                if (*ch == '.') f = 1;
                else if (*ch >= '0' && *ch <= '9') {
                    if (!isfinite(employee->salary)) {
                        while (*ch != '\n' && *ch != EOF) *ch = fgetc(input);
                        *ch = fgetc(input);
                        return Overflow; // Проверка переполнения
                    }
                    employee->salary = employee->salary * 10 + (*ch - '0');
                    if (f) after_dot++;
                } else {
                    while (*ch != '\n' && *ch != EOF && *ch != '\0') *ch = fgetc(input);
                    *ch = fgetc(input);
                    return Incorrect_salary;
                }
                *ch = fgetc(input);
            }
            if (after_dot > 0.0) {
                employee->salary /= pow(10, after_dot);
            }
            if (employee->salary < 0.0) return Incorrect_salary; // Проверка на отрицательность
            next_lexem(ch, input);
            count_of_lex++;
        }
    }
    return (count_of_lex == 5) ? Normal : Data_emp_err;
}


int get_Employees(FILE *input, Employee** table, int* count){
    if (!input){
        return File_not_found;
    }
    int ch;
    int capipacity = 2;
    *table = malloc(capipacity * sizeof(Employee));
    next_lexem(&ch, input);
    Employee employee = {0, "", "", 0};
    while (1){
        int res = get_Emp(input, &employee, &ch);
        if (res == EOF){
            break;
        }
        if (res != Normal){
            printf("ERR: %d\n", res);
            continue;
        }else{
            (*table)[*count] = employee;
            (*count)++;
        }
        if (*count == capipacity){
            capipacity *= 2; // исправить
            Employee* temp = realloc(*table, sizeof(Employee) * capipacity);
            if (temp == NULL) {
                free(*table);
                return Malloc_err;
            }
            *table = temp;
        }
    }
    return Normal;
}
int write_arr(FILE* out, Employee* table, int count){
    if(!out) return File_not_found;

    for (int i = 0; count > i; i++) {
        int print_status = fprintf(out, "%lli %s %s %lf\n", table[i].id, table[i].name, table[i].surname, table[i].salary);
        if(print_status < 0) {
            fclose(out);
            return Write_file_err;
        }
    }

    return Normal;
}

int main(int argc, char** argv) {
    Employee* table;
    kOpts opt;
    int ch = 0;
    char out_path[PATH_MAX], in_path[PATH_MAX];
    int count = 0;
    Err result = GetOpts(argc, argv, &opt, in_path, out_path);
    if(result != Normal){
        switch (result){
            case Not_enough_arg:
                printf("Not enough number of argumnets.\n");
                break;
            case Too_many_arg:
                printf("Too many argumnets.\n");
                break;
            case Equal_path:
                printf("Files are the same \n");
                break;
            case Incorrect_input:
                printf("Invalid value of first arguement. The first arguement must be a flag with starting with '-' or '/'.\n");
                break;
            case Unknown_flag:
                printf("Unknown flag type.\n");
                break;
            default:
                break;
        }
        return 1;
    }

    FILE* input = fopen(in_path, "r");
    FILE* out = fopen(out_path, "w");
    if (input == NULL) {
        printf("File not found\n");
        return 1;
    }
    if (out == NULL) {
        printf("File not found\n");
        return 1;
    }

    if (get_Employees(input, &table, &count) != Normal) {
        fclose(input);
        free(table);
        printf("Error reading employees\n");
        return 1;
    }
    switch (opt){
        case OPT_A:
            qsort(table, count, sizeof(Employee), comparator_a);
            break;
        case OPT_D:
            qsort(table, count, sizeof(Employee), comparator_d);
            break;
        default:
            break;
    }
    write_arr(out, table, count);
    fclose(input);
    fclose(out);
    free(table);
    return 0;
}