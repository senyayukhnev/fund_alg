#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

typedef struct Allocator Allocator;

Allocator* allocator_create(void* memory, size_t size);
void allocator_destroy(Allocator* allocator);
void* allocator_alloc(Allocator* allocator, size_t size);
void allocator_free(Allocator* allocator, void* memory);

typedef struct {
    int type;
    const char* context;
    const char* message;
} error_msg;

enum {
    SUCCESS = 0,
    INCORRECT_OPTIONS_ERROR,
    MEMORY_ALLOCATED_ERROR
};

void write_str(const char* str) {
    write(1, str, strlen(str));
}

void write_number(int num) {
    char buffer[20];
    int length = 0;

    if (num == 0) {
        buffer[length++] = '0';
    } else {
        if (num < 0) {
            buffer[length++] = '-';
            num = -num;
        }
        int temp = num;
        while (temp > 0) {
            temp /= 10;
            length++;
        }
        for (int i = length - 1; num > 0; --i) {
            buffer[i] = '0' + (num % 10);
            num /= 10;
        }
    }
    write(1, buffer, length);
}

int print_error(error_msg error) {
    write_str("[");
    write_str(error.context);
    write_str("] ");
    write_str(error.message);
    write_str("\n");
    return EXIT_FAILURE;
}

error_msg init_library(void* library) {
    Allocator* (*allocator_create)(void*, size_t) = dlsym(library, "allocator_create");
    if (!allocator_create) {
        return (error_msg) {INCORRECT_OPTIONS_ERROR, "main", "failed to find allocator_create"};
    }

    void (*allocator_destroy)(Allocator*) = dlsym(library, "allocator_destroy");
    if (!allocator_destroy) {
        return (error_msg) {INCORRECT_OPTIONS_ERROR, "main", "failed to find allocator_destroy"};
    }

    void* (*allocator_alloc)(Allocator*, size_t) = dlsym(library, "allocator_alloc");
    if (!allocator_alloc) {
        return (error_msg) {INCORRECT_OPTIONS_ERROR, "main", "failed to find allocator_alloc"};
    }

    void (*allocator_free)(Allocator*, void*) = dlsym(library, "allocator_free");
    if (!allocator_free) {
        return (error_msg) {INCORRECT_OPTIONS_ERROR, "main", "failed to find allocator_free"};
    }

    return (error_msg) {SUCCESS, "", ""};
}

void test_allocator(Allocator* (*allocator_create)(void*, size_t),
                    void (*allocator_destroy)(Allocator*),
                    void* (*allocator_alloc)(Allocator*, size_t),
                    void (*allocator_free)(Allocator*, void*)) {
    size_t memory_size = 1024 * 1024;
    void* memory = malloc(memory_size);
    Allocator* allocator = allocator_create(memory, memory_size);

    if (!allocator) {
        print_error((error_msg) {MEMORY_ALLOCATED_ERROR, "test_allocator", "Failed to create allocator."});
        free(memory);
        return;
    }

    write_str("Allocator created successfully.\n");

    // Тест 1: Выделение и освобождение памяти
    write_str("Test 1: Allocating and freeing memory...\n");
    void* block1 = allocator_alloc(allocator, 32);
    if (!block1) {
        print_error((error_msg) {MEMORY_ALLOCATED_ERROR, "test_allocator", "Failed to allocate small block."});
    } else {
        write_str("Small block allocated successfully.\n");
    }
    allocator_free(allocator, block1);
    write_str("Small block freed successfully.\n");

    // Тест 2: Выделение памяти большего размера
    write_str("Test 2: Allocating larger memory block...\n");
    void* block2 = allocator_alloc(allocator, 128);
    if (!block2) {
        print_error((error_msg) {MEMORY_ALLOCATED_ERROR, "test_allocator", "Failed to allocate medium block."});
    } else {
        write_str("Medium block allocated successfully.\n");
    }
    allocator_free(allocator, block2);
    write_str("Medium block freed successfully.\n");

    // Тест 3: Повторное выделение памяти после освобождения
    write_str("Test 3: Reallocating memory after freeing...\n");
    void* block3 = allocator_alloc(allocator, 64);
    if (!block3) {
        print_error((error_msg) {MEMORY_ALLOCATED_ERROR, "test_allocator", "Failed to allocate block after free."});
    } else {
        write_str("Block allocated successfully after freeing.\n");
    }
    allocator_free(allocator, block3);
    write_str("Block freed successfully after realloc.\n");

    // Тест 4: Попытка выделения слишком большого блока памяти
    write_str("Test 4: Attempting to allocate too much memory...\n");
    void* block4 = allocator_alloc(allocator, memory_size + 1); // Попытка выделить больше, чем доступно
    if (block4 != NULL) {
        print_error((error_msg) {MEMORY_ALLOCATED_ERROR, "test_allocator", "Unexpectedly allocated too much memory."});
    } else {
        write_str("Attempted allocation failed as expected.\n");
    }

    // Тест 5: Выделение нескольких блоков памяти
    write_str("Test 5: Allocating multiple memory blocks...\n");
    void* block5 = allocator_alloc(allocator, 32);
    void* block6 = allocator_alloc(allocator, 64);
    if (!block5 || !block6) {
        print_error((error_msg) {MEMORY_ALLOCATED_ERROR, "test_allocator", "Failed to allocate multiple blocks."});
    } else {
        write_str("Multiple blocks allocated successfully.\n");
    }
    allocator_free(allocator, block5);
    allocator_free(allocator, block6);
    write_str("Multiple blocks freed successfully.\n");

    allocator_destroy(allocator);
    free(memory);
    write_str("Allocator destroyed successfully.\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_error((error_msg) {INCORRECT_OPTIONS_ERROR, "main", "Usage: <program> <path_to_allocator_library>"});
        return EXIT_FAILURE;
    }

    void* handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        write_str("Failed to load library: ");
        write_str(dlerror());
        write_str("\n");
        return EXIT_FAILURE;
    }

    error_msg error = init_library(handle);
    if (error.type != SUCCESS) {
        dlclose(handle);
        return print_error(error);
    }

    write_str("Testing allocator...\n");

    Allocator* (*allocator_create)(void*, size_t) = dlsym(handle, "allocator_create");
    void (*allocator_destroy)(Allocator*) = dlsym(handle, "allocator_destroy");
    void* (*allocator_alloc)(Allocator*, size_t) = dlsym(handle, "allocator_alloc");
    void (*allocator_free)(Allocator*, void*) = dlsym(handle, "allocator_free");

    test_allocator(allocator_create, allocator_destroy, allocator_alloc, allocator_free);

    dlclose(handle);
    return EXIT_SUCCESS;
}
