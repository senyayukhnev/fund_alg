#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct TreeNode {
    char data;                     // Данные узла
    struct TreeNode* firstChild;  // Указатель на первого ребенка
    struct TreeNode* nextSibling; // Указатель на следующего брата
} TreeNode;

typedef enum {
    Normal,
    Not_enough_arg,
    Too_many_arg,
    Incorrect_input,
    Unknown_flag,
    File_not_found,
    Equal_path,
    Malloc_err,
    Overflow,
} Err;

Err check_file_names(const char *file1, const char *file2) {
    const char *name1 = strrchr(file1, '\\');
    const char *name2 = strrchr(file2, '\\');
    if (name1 != NULL) name1++;
    else name1 = file1;
    if (name2 != NULL) name2++;
    else name2 = file2;

    if (strcmp(name1, name2) == 0) return Equal_path;
    return Normal;
}

int GetOpts(int argc, char** argv, char* in_path, char* out_path) {
    if (argc > 3) return Too_many_arg;
    if (argc < 3) return Not_enough_arg;
    strcpy(in_path, argv[1]);
    strcpy(out_path, argv[2]);

    if (check_file_names(in_path, out_path)) return Equal_path;
    return Normal;
}

TreeNode* createNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) return NULL;
    newNode->data = data;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

void addChild(TreeNode* parent, TreeNode* child) {
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        TreeNode* sibling = parent->firstChild;
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = child;
    }
}

void freeTree(TreeNode* node) {
    if (node == NULL) return;
    freeTree(node->firstChild);
    freeTree(node->nextSibling);
    free(node);
}

TreeNode* parseTree(const char* str, int* index) {
    while (str[*index] == ' ') (*index)++;
    if (str[*index] == '\0' || str[*index] == ')') return NULL;

    TreeNode* node = createNode(str[*index]);
    if (node == NULL) return NULL;
    (*index)++;

    if (str[*index] == '(') {
        (*index)++;
        node->firstChild = parseTree(str, index);
    }

    TreeNode* current = node;
    while (str[*index] == ',') {
        (*index)++;
        current->nextSibling = parseTree(str, index);
        current = current->nextSibling;
    }

    if (str[*index] == ')') (*index)++;
    return node;
}

int get_Trees(FILE *input, TreeNode*** table, int* count) {
    if (!input) return File_not_found;
    int capacity = 2;
    *table = malloc(capacity * sizeof(TreeNode*));
    if (*table == NULL) return Malloc_err;

    char str[512];
    while (fgets(str, sizeof(str), input) != NULL) {
        if (*count == capacity) {
            capacity *= 2;
            TreeNode** temp = realloc(*table, sizeof(TreeNode*) * capacity);
            if (temp == NULL) {
                free(*table);
                return Malloc_err;
            }
            *table = temp;
        }
        int index = 0;
        (*table)[*count] = parseTree(str, &index);
        if ((*table)[*count] == NULL) return Malloc_err;
        (*count)++;
    }

    return Normal;
}

void printTree(FILE* output, TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level - 1; i++) fprintf(output, "    ");
    if (level > 0) fprintf(output, "|-- ");
    fprintf(output, "%c\n", node->data);

    if (node->firstChild != NULL) {
        for (int i = 0; i < level; i++) fprintf(output, "    ");
        fprintf(output, "|\n");
    }

    printTree(output, node->firstChild, level + 1);
    printTree(output, node->nextSibling, level);
}

int main(int argc, char** argv) {
    TreeNode** table;
    int count = 0;
    char out_path[PATH_MAX], in_path[PATH_MAX];
    Err result = GetOpts(argc, argv, in_path, out_path);
    if (result != Normal) {
        switch (result) {
            case Not_enough_arg:
                printf("Not enough number of arguments.\n");
                break;
            case Too_many_arg:
                printf("Too many arguments.\n");
                break;
            case Equal_path:
                printf("Files are the same\n");
                break;
            default:
                break;
        }
        return 1;
    }

    FILE* input = fopen(in_path, "r");
    FILE* output = fopen(out_path, "w");
    if (input == NULL || output == NULL) {
        printf("File not found\n");
        return 1;
    }

    if (get_Trees(input, &table, &count) != Normal) {
        fclose(input);
        fclose(output);
        free(table);
        printf("Error loading trees\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(output, "Tree %d:\n", i + 1);
        printTree(output, table[i], 0);
        freeTree(table[i]);
        fprintf(output, "\n");
    }

    fclose(input);
    fclose(output);
    free(table);
    return 0;
}
