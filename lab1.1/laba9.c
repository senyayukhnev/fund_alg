#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int GetOpts(int argc, char **argv, int number[]) {
    if (argc != 3) {
        return 1;
    }
    for (int i = 1; i <= 2; ++i) {
        const char *proceeding_option = argv[i];
        int sign = 1;
        for (int j = 0; proceeding_option[j]; ++j) {
            if (proceeding_option[j] >= '0' && proceeding_option[j] <= '9') {
                int v_number = number[i - 1];
                number[i - 1] *= 10;
                if (number[i - 1] / 10 != v_number) return 1;
                if (sign == 1)
                    if (number[i - 1] > INT_MAX - (proceeding_option[j] - '0')) return 1;
                if (number[i - 1] < INT_MIN + (proceeding_option[j] - '0')) return 1;
                number[i - 1] += sign * (proceeding_option[j] - '0');
            } else if (proceeding_option[j] == '-' && !j)
                sign = -1;
            else
                return 1;
        }

    }
    if (number[0] > number[1]) return 1;
    return 0;
}

int compare(const void *a, const void *b) {
    int arg1 = *(int*)a;
    int arg2 = *(int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int bin_search(int const array[], int size, int element){
    int a = 0;
    int b = size - 1;
    int m;
    while (a <= b){
        m = (a + b) / 2;
        if (array[m] > element) b = m - 1;
        else if (array[m] < element) a = m + 1;
        else return array[m];
    }
    return array[m];
}

int Random_Sequence_Generator_static(int a, int b, int size, int array[]) {
    for (int i = 0; i < size; i++) {
        array[i] = a + rand() % (b - a + 1);
    }
    return 0;
}

void Random_Sequence_Generator_dynamic(int *a, int *b, int size_a, int size_b){
    for (int i = 0; i < size_a; i++){
        a[i] = -1000 + rand() % 2001;
    }
    for (int i = 0; i < size_b; i++){
        b[i] = -1000 + rand() % 2001;
    }
}

void task1(int size, int array[], int ans[]){
    int index_max = 0;
    int index_min = 0;
    for (int i = 0; i < size; i++){
        if (ans[0] < array[i]){
            ans[0] = array[i];
            index_max = i;
        }
        if (ans[1] >= array[i]){
            ans[1] = array[i];
            index_min = i;
        }
    }
    array[index_max] = ans[1];
    array[index_min] = ans[0];
}
void pprint(int array[], int size){
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void task2(int a[], int b[], int sa, int sb, int c[]){
    qsort(b, sb, sizeof(int), compare);
    for (int i = 0; i < sa; i++){
        c[i] = a[i] + bin_search(b, sb, a[i]);
    }
}
int main(int arc, char **argv){
    int max_min[2] = {INT_MIN, INT_MAX};
    int number[2] = {0, 0};
    int const size = 100;
    int array[size];

    srand(time(NULL));

    if (GetOpts(arc, argv, number)){
        printf("Incorrect option");
        return 1;
    }

    Random_Sequence_Generator_static(number[0], number[1], size, array);

    pprint(array, size);

    task1(size, array, max_min);

    pprint(array, size);

    printf("Max: %d, Min: %d\n", max_min[0], max_min[1]);

    int *a = NULL;
    int *b = NULL;
    int size_a = 10 + rand() % 9991;
    int size_b = 10 + rand() % 9991;
    a = (int *) malloc(size_a * sizeof(int));
    if (a == NULL){
        printf("Memory allocation error");
        return 1;
    }
    b = (int *) malloc(size_b * sizeof(int));
    if (b == NULL){
        free(a);
        printf("Memory allocation error");
        return 1;
    }

    Random_Sequence_Generator_dynamic(a, b, size_a, size_b);

    printf("A: ");
    pprint(a, size_a);
    printf("B: ");
    pprint(b, size_b);

    int *c = (int *) malloc(size_a * sizeof(int));
    if (c == NULL){
        printf("Memory allocation error");
        free(a);
        free(b);
        return 1;

    }
    task2(a, b, size_a, size_b, c);
    printf("Array C Array A Close A\n");
    for (int i = 0; i < size_a; i++){
        printf("%5d, %5d, %5d\n", c[i], a[i], c[i] - a[i]);
    }


    free(a);
    free(b);
    free(c);
    return 0;
}

