#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LEVELS 32

// Узел дерева аллокатора
typedef struct BuddyNode {
    size_t size;                    // Размер блока
    int is_free;                    // Свободен ли блок
    struct BuddyNode* next;         // Указатель на следующий блок в списке
} BuddyNode;

// Структура аллокатора
typedef struct Allocator {
    void* memory;                   // Начало памяти, управляемой аллокатором
    size_t size;                    // Общий размер памяти
    size_t min_block_size;          // Минимальный размер блока
    BuddyNode* free_lists[MAX_LEVELS]; // Списки свободных блоков на каждом уровне
} Allocator;

// Рассчитывает ближайшую степень двойки, большую или равную size
size_t next_power_of_two(size_t size) {
    size_t power = 1;
    while (power < size) {
        power *= 2;
    }
    return power;
}

// Вычисляет уровень дерева для заданного размера блока
int calculate_level(size_t block_size, size_t min_block_size) {
    int level = 0;
    while (block_size > min_block_size) {
        block_size /= 2;
        level++;
    }
    return level;
}

// Инициализация аллокатора
Allocator* allocator_create(void* const memory, const size_t size) {
    if (!memory || size < sizeof(BuddyNode)) {
        return NULL;
    }

    Allocator* allocator = (Allocator*)malloc(sizeof(Allocator));
    if (!allocator) {
        return NULL;
    }

    allocator->memory = memory;
    allocator->size = next_power_of_two(size);
    allocator->min_block_size = 16; // Минимальный размер блока (например, 16 байт)

    for (int i = 0; i < MAX_LEVELS; i++) {
        allocator->free_lists[i] = NULL;
    }

    // Инициализируем весь регион как один свободный блок
    BuddyNode* initial_block = (BuddyNode*)memory;
    initial_block->size = allocator->size;
    initial_block->is_free = 1;
    initial_block->next = NULL;

    int level = calculate_level(initial_block->size, allocator->min_block_size);
    allocator->free_lists[level] = initial_block;

    return allocator;
}

// Деинициализация аллокатора
void allocator_destroy(Allocator* const allocator) {
    if (allocator) {
        free(allocator);
    }
}

// Разделение блока на два будди-блока
void split_block(Allocator* allocator, BuddyNode* block, int level) {
    size_t new_block_size = block->size / 2;

    BuddyNode* buddy = (BuddyNode*)((char*)block + new_block_size);
    buddy->size = new_block_size;
    buddy->is_free = 1;
    buddy->next = allocator->free_lists[level + 1];

    allocator->free_lists[level + 1] = buddy;

    block->size = new_block_size;
    block->next = buddy;
}

// Выделение памяти
void* allocator_alloc(Allocator* const allocator, const size_t size) {
    if (!allocator || size == 0 || size > allocator->size) {
        return NULL;
    }

    size_t aligned_size = next_power_of_two(size);
    int level = calculate_level(aligned_size, allocator->min_block_size);

    for (int current_level = level; current_level < MAX_LEVELS; current_level++) {
        if (allocator->free_lists[current_level]) {
            BuddyNode* block = allocator->free_lists[current_level];
            allocator->free_lists[current_level] = block->next;

            while (current_level > level) {
                split_block(allocator, block, --current_level);
            }

            block->is_free = 0;
            return (void*)((char*)block + sizeof(BuddyNode));
        }
    }

    return NULL; // Нет подходящего блока
}

// Освобождение памяти
void allocator_free(Allocator* const allocator, void* const memory) {
    if (!allocator || !memory) {
        return;
    }

    BuddyNode* block = (BuddyNode*)((char*)memory - sizeof(BuddyNode));
    block->is_free = 1;

    int level = calculate_level(block->size, allocator->min_block_size);
    block->next = allocator->free_lists[level];
    allocator->free_lists[level] = block;

    // Попытка слияния с будди-блоком
    size_t block_offset = (char*)block - (char*)allocator->memory;
    size_t buddy_offset = block_offset ^ block->size;
    BuddyNode* buddy = (BuddyNode*)((char*)allocator->memory + buddy_offset);

    if (buddy->is_free && buddy->size == block->size) {
        // Удалить buddy из списка
        BuddyNode** current = &allocator->free_lists[level];
        while (*current && *current != buddy) {
            current = &(*current)->next;
        }
        if (*current == buddy) {
            *current = buddy->next;
        }

        // Слияние
        if (buddy < block) {
            block = buddy;
        }
        block->size *= 2;
        allocator_free(allocator, (void*)((char*)block + sizeof(BuddyNode)));
    }
}
