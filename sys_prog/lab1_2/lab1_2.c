#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_FILES 100
#define MAX_PATH_LEN 1024
#define BUFFER_SIZE 4096

// Перечисление возможных ошибок
typedef enum {
    OK = 0,
    ERR_INVALID_ARGS,
    ERR_FILE_OPEN,
    ERR_MEMORY,
    ERR_FORK,
    ERR_FILE_WRITE,
    ERR_INVALID_N,
    ERR_INVALID_HEX,
    ERR_INVALID_NUM,
    ERR_STRING_NOT_FOUND
} ErrorCode;

// Функция для вывода статуса ошибки
void print_status(ErrorCode code, const char *additional_info) {
    const char *messages[] = {
            "Success",
            "Invalid arguments",
            "Failed to open file",
            "Memory allocation failed",
            "Fork failed",
            "Failed to write to file",
            "Invalid N for operation",
            "Invalid hexadecimal value",
            "Invalid number",
            "Search string not found in any files"
    };

    if (code != OK) {
        fprintf(stderr, "Error: %s", messages[code]);
        if (additional_info) {
            fprintf(stderr, " (%s)", additional_info);
        }
        fprintf(stderr, "\n");
    }
}

// Функция для проверки, является ли строка шестнадцатеричным числом
int is_hex(const char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (!((str[i] >= '0' && str[i] <= '9') ||
              (str[i] >= 'A' && str[i] <= 'F') ||
              (str[i] >= 'a' && str[i] <= 'f'))) {
            return 0;
        }
    }
    return 1;
}

// Функция для проверки, является ли строка числом
int is_number(const char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

// Функция для операции xorN
ErrorCode xor_operation(const char *filename, int n) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return ERR_FILE_OPEN;
    }

    size_t block_size = 1 << n; // 2^N байт
    uint8_t *block = (uint8_t *)calloc(block_size, sizeof(uint8_t));
    if (!block) {
        fclose(file);
        return ERR_MEMORY;
    }

    size_t bytes_read;
    uint8_t buffer[BUFFER_SIZE];
    size_t total_bytes = 0;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file))) {
        for (size_t i = 0; i < bytes_read; i++) {
            block[total_bytes % block_size] ^= buffer[i];
            total_bytes++;
        }
    }

    // Дополняем нулями если нужно
    if (total_bytes % block_size != 0) {
        for (size_t i = total_bytes % block_size; i < block_size; i++) {
            block[i] ^= 0x00;
        }
    }

    printf("XOR result for %s (N=%d): ", filename, n);
    for (size_t i = 0; i < block_size; i++) {
        printf("%02x ", block[i]);
    }
    printf("\n");

    free(block);
    fclose(file);
    return OK;
}

// Функция для операции mask
ErrorCode mask_operation(const char *filename, uint32_t mask) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return ERR_FILE_OPEN;
    }

    uint32_t value;
    size_t count = 0;
    size_t total_values = 0;

    while (fread(&value, sizeof(uint32_t), 1, file) == 1) {
        if ((value & mask) == mask) {
            count++;
        }
        total_values++;
    }

    printf("File %s: %zu matching values out of %zu (mask: 0x%08x)\n",
           filename, count, total_values, mask);

    fclose(file);
    return OK;
}

// Функция для операции copyN
ErrorCode copy_operation(const char *filename, int n) {
    for (int i = 1; i <= n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            return ERR_FORK;
        } else if (pid == 0) { // Дочерний процесс
            char new_filename[MAX_PATH_LEN];
            snprintf(new_filename, MAX_PATH_LEN, "%s_%d", filename, i);

            FILE *src = fopen(filename, "rb");
            if (!src) {
                exit(ERR_FILE_OPEN);
            }

            FILE *dst = fopen(new_filename, "wb");
            if (!dst) {
                fclose(src);
                exit(ERR_FILE_OPEN);
            }

            uint8_t buffer[BUFFER_SIZE];
            size_t bytes_read;

            while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
                if (fwrite(buffer, 1, bytes_read, dst) != bytes_read) {
                    fclose(src);
                    fclose(dst);
                    exit(ERR_FILE_WRITE);
                }
            }

            fclose(src);
            fclose(dst);
            exit(OK);
        }
    }

    // Ожидаем завершения всех дочерних процессов
    ErrorCode overall_status = OK;
    for (int i = 1; i <= n; i++) {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            ErrorCode child_status = WEXITSTATUS(status);
            if (child_status == OK) {
                printf("Created copy %d of %s\n", i, filename);
            } else {
                print_status(child_status, filename);
                overall_status = child_status;
            }
        } else {
            print_status(ERR_FORK, filename);
            overall_status = ERR_FORK;
        }
    }

    return overall_status;
}

// Функция для операции find
ErrorCode find_operation(const char *filename, const char *search_string) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ERR_FILE_OPEN;
    }

    char line[BUFFER_SIZE];
    int found = 0;

    while (fgets(line, BUFFER_SIZE, file)) {
        if (strstr(line, search_string)) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Found in: %s\n", filename);
        return OK;
    } else {
        return ERR_STRING_NOT_FOUND;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_status(ERR_INVALID_ARGS, "Usage: file1 [file2 ...] command [args]");
        return ERR_INVALID_ARGS;
    }

    int num_files = argc - 2;
    char *command = argv[argc - 1];
    char *command_arg = NULL;

    // Проверяем, есть ли аргумент у команды
    if (strcmp(command, "xor2") != 0 && strcmp(command, "xor3") != 0 &&
        strcmp(command, "xor4") != 0 && strcmp(command, "xor5") != 0 &&
        strcmp(command, "xor6") != 0 && strcmp(command, "copyN") != 0) {
        if (argc < 4) {
            print_status(ERR_INVALID_ARGS, command);
            return ERR_INVALID_ARGS;
        }
        num_files = argc - 3;
        command_arg = argv[argc - 1];
        command = argv[argc - 2];
    }

    ErrorCode status = OK;

    // Обработка команд
    if (strncmp(command, "xor", 3) == 0) {
        int n = command[3] - '0';
        if (n < 2 || n > 6) {
            print_status(ERR_INVALID_N, command);
            return ERR_INVALID_N;
        }

        for (int i = 1; i <= num_files; i++) {
            status = xor_operation(argv[i], n);
            if (status != OK) {
                print_status(status, argv[i]);
            }
        }
    }
    else if (strcmp(command, "mask") == 0) {
        if (!command_arg || !is_hex(command_arg)) {
            print_status(ERR_INVALID_HEX, command_arg);
            return ERR_INVALID_HEX;
        }

        uint32_t mask = strtoul(command_arg, NULL, 16);
        for (int i = 1; i <= num_files; i++) {
            status = mask_operation(argv[i], mask);
            if (status != OK) {
                print_status(status, argv[i]);
            }
        }
    }
    else if (strcmp(command, "copyN") == 0) {
        if (!command_arg || !is_number(command_arg)) {
            print_status(ERR_INVALID_NUM, command_arg);
            return ERR_INVALID_NUM;
        }

        int n = atoi(command_arg);
        if (n <= 0) {
            print_status(ERR_INVALID_NUM, command_arg);
            return ERR_INVALID_NUM;
        }

        for (int i = 1; i <= num_files; i++) {
            status = copy_operation(argv[i], n);
            if (status != OK) {
                print_status(status, argv[i]);
            }
        }
    }
    else if (strcmp(command, "find") == 0) {
        if (!command_arg) {
            print_status(ERR_INVALID_ARGS, "Missing search string");
            return ERR_INVALID_ARGS;
        }

        int any_found = 0;
        for (int i = 1; i <= num_files; i++) {
            pid_t pid = fork();
            if (pid == -1) {
                print_status(ERR_FORK, argv[i]);
                status = ERR_FORK;
                continue;
            } else if (pid == 0) { // Дочерний процесс
                ErrorCode child_status = find_operation(argv[i], command_arg);
                exit(child_status);
            } else {
                int child_status;
                waitpid(pid, &child_status, 0);
                if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == OK) {
                    any_found = 1;
                }
            }
        }

        if (!any_found) {
            print_status(ERR_STRING_NOT_FOUND, command_arg);
            status = ERR_STRING_NOT_FOUND;
        }
    }
    else {
        print_status(ERR_INVALID_ARGS, command);
        return ERR_INVALID_ARGS;
    }

    return status;
}