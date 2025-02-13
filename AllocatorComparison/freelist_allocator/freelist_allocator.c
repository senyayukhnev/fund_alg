//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stddef.h>
//
//// Структура свободного блока
//typedef struct FreeBlock {
//    size_t size;                     // Размер блока
//    struct FreeBlock* next;          // Указатель на следующий свободный блок
//} FreeBlock;
//
//// Структура аллокатора
//typedef struct Allocator {
//    void* memory;                    // Начало памяти, управляемой аллокатором
//    size_t size;                     // Общий размер памяти
//    FreeBlock* free_list;            // Список свободных блоков
//} Allocator;
//
//// Инициализация аллокатора
//Allocator* allocator_create(void* const memory, const size_t size) {
//    if (!memory || size < sizeof(FreeBlock)) {
//        return NULL;
//    }
//
//    Allocator* allocator = (Allocator*)malloc(sizeof(Allocator));
//    if (!allocator) {
//        return NULL;
//    }
//
//    allocator->memory = memory;
//    allocator->size = size;
//    allocator->free_list = (FreeBlock*)memory;
//
//    allocator->free_list->size = size;
//    allocator->free_list->next = NULL;
//
//    return allocator;
//}
//
//// Деинициализация аллокатора
//void allocator_destroy(Allocator* const allocator) {
//    if (allocator) {
//        free(allocator);
//    }
//}
//
//// Выделение памяти
//void* allocator_alloc(Allocator* const allocator, const size_t size) {
//    if (!allocator || size == 0) {
//        return NULL;
//    }
//
//    FreeBlock* prev = NULL;
//    FreeBlock* current = allocator->free_list;
//
//    while (current) {
//        if (current->size >= size + sizeof(FreeBlock)) {
//            // Найден подходящий блок
//            void* allocated_memory = (void*)((char*)current + sizeof(FreeBlock));
//            size_t remaining_size = current->size - size - sizeof(FreeBlock);
//
//            if (remaining_size >= sizeof(FreeBlock)) {
//                // Разделить блок
//                FreeBlock* next_block = (FreeBlock*)((char*)current + size + sizeof(FreeBlock));
//                next_block->size = remaining_size;
//                next_block->next = current->next;
//
//                if (prev) {
//                    prev->next = next_block;
//                } else {
//                    allocator->free_list = next_block;
//                }
//            } else {
//                // Блок слишком мал для разделения
//                if (prev) {
//                    prev->next = current->next;
//                } else {
//                    allocator->free_list = current->next;
//                }
//            }
//
//            current->size = size; // Установить размер выделенного блока
//            return allocated_memory;
//        }
//
//        prev = current;
//        current = current->next;
//    }
//
//    return NULL; // Нет подходящего блока
//}
//
//// Освобождение памяти
//void allocator_free(Allocator* const allocator, void* const memory) {
//    if (!allocator || !memory) {
//        return;
//    }
//
//    FreeBlock* block_to_free = (FreeBlock*)((char*)memory - sizeof(FreeBlock));
//    FreeBlock* current = allocator->free_list;
//    FreeBlock* prev = NULL;
//
//    while (current && current < block_to_free) {
//        prev = current;
//        current = current->next;
//    }
//
//    // Вставить освобожденный блок в список
//    block_to_free->next = current;
//    if (prev) {
//        prev->next = block_to_free;
//    } else {
//        allocator->free_list = block_to_free;
//    }
//
//    // Слияние с соседними блоками
//    if ((char*)block_to_free + block_to_free->size + sizeof(FreeBlock) == (char*)current) {
//        block_to_free->size += current->size + sizeof(FreeBlock);
//        block_to_free->next = current->next;
//    }
//
//    if (prev && (char*)prev + prev->size + sizeof(FreeBlock) == (char*)block_to_free) {
//        prev->size += block_to_free->size + sizeof(FreeBlock);
//        prev->next = block_to_free->next;
//    }
//}
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// Структура свободного блока
typedef struct FreeBlock {
    size_t size;                     // Размер блока
    struct FreeBlock* next;          // Указатель на следующий свободный блок
} FreeBlock;

// Структура аллокатора
typedef struct Allocator {
    size_t size;                     // Общий размер памяти
    FreeBlock* free_list;            // Список свободных блоков
} Allocator;

// Инициализация аллокатора
Allocator* allocator_create(void* const memory, const size_t size) {
    if (!memory || size < sizeof(Allocator) + sizeof(FreeBlock)) {
        return NULL;
    }

    // Разместить структуру Allocator в начале предоставленной памяти
    Allocator* allocator = (Allocator*)memory;
    allocator->size = size;

    // Инициализация списка свободных блоков
    allocator->free_list = (FreeBlock*)((char*)memory + sizeof(Allocator));
    allocator->free_list->size = size - sizeof(Allocator);
    allocator->free_list->next = NULL;

    return allocator;
}

// Деинициализация аллокатора (здесь ничего делать не нужно)
void allocator_destroy(Allocator* const allocator) {
    // Не требуется действий, так как память управляется пользователем
}

// Выделение памяти
void* allocator_alloc(Allocator* const allocator, const size_t size) {
    if (!allocator || size == 0) {
        return NULL;
    }

    FreeBlock* prev = NULL;
    FreeBlock* current = allocator->free_list;

    while (current) {
        if (current->size >= size + sizeof(FreeBlock)) {
            // Найден подходящий блок
            void* allocated_memory = (void*)((char*)current + sizeof(FreeBlock));
            size_t remaining_size = current->size - size - sizeof(FreeBlock);

            if (remaining_size >= sizeof(FreeBlock)) {
                // Разделить блок
                FreeBlock* next_block = (FreeBlock*)((char*)current + size + sizeof(FreeBlock));
                next_block->size = remaining_size;
                next_block->next = current->next;

                if (prev) {
                    prev->next = next_block;
                } else {
                    allocator->free_list = next_block;
                }
            } else {
                // Блок слишком мал для разделения
                if (prev) {
                    prev->next = current->next;
                } else {
                    allocator->free_list = current->next;
                }
            }

            current->size = size; // Установить размер выделенного блока
            return allocated_memory;
        }

        prev = current;
        current = current->next;
    }

    return NULL; // Нет подходящего блока
}

// Освобождение памяти
void allocator_free(Allocator* const allocator, void* const memory) {
    if (!allocator || !memory) {
        return;
    }

    FreeBlock* block_to_free = (FreeBlock*)((char*)memory - sizeof(FreeBlock));
    FreeBlock* current = allocator->free_list;
    FreeBlock* prev = NULL;

    while (current && current < block_to_free) {
        prev = current;
        current = current->next;
    }

    // Вставить освобожденный блок в список
    block_to_free->next = current;
    if (prev) {
        prev->next = block_to_free;
    } else {
        allocator->free_list = block_to_free;
    }

    // Слияние с соседними блоками
    if ((char*)block_to_free + block_to_free->size + sizeof(FreeBlock) == (char*)current) {
        block_to_free->size += current->size + sizeof(FreeBlock);
        block_to_free->next = current->next;
    }

    if (prev && (char*)prev + prev->size + sizeof(FreeBlock) == (char*)block_to_free) {
        prev->size += block_to_free->size + sizeof(FreeBlock);
        prev->next = block_to_free->next;
    }
}
