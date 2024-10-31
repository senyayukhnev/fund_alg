#include <stdio.h>
#include <stdlib.h>

typedef enum keyErrs {
    MallocErr,
    BaseErr,
    NumErr,
    Normal
} keyErrs;

unsigned int decrement(unsigned int num) { //   00000100
    unsigned int mask = 1; //                   00000001
    while ((num & mask) == 0) {               //00000000 // 00000000 // выход из цикла
        num = num ^ mask;                     //00000101 // 00000111
        mask <<= 1;                           //00000010 // 00000100
    }
    return num ^ mask;                        //00000011
}

unsigned int increment(unsigned int num) {
    unsigned int mask = 1;
    while (num & mask) {
        num = num ^ mask;
        mask <<= 1;
    }
    return num ^ mask; 
}

keyErrs convert_to_base_2r(int num, unsigned int r, char **result){
    if (num < 0) {
        return NumErr;
    }
    else if (num == 0) {
        *result = malloc(sizeof(char) << 1);
        if (*result == NULL) return MallocErr;

        (*result)[0] = '0';
        (*result)[1] = '\0';
        return Normal;
    }
    if (r > 5 || r < 1) return BaseErr;

    unsigned int base = 1 << r;
    unsigned int mask = decrement(base);

    unsigned int size = sizeof(char) << 8;
    char *out = (char*)malloc(size);
    if(out == NULL) return MallocErr;

    unsigned int index = 0;
    const char chars[] = "0123456789abcdefghijklmnopqrstuv";

    while (num > 0){
        unsigned int current_bits = num & mask;
        out[index] = chars[current_bits];
        index = increment(index);
        num >>= r;
    }

    unsigned int left = 0;
    unsigned int right = decrement(index);
    while(left < right) {
        char temp = out[left];
        out[left] = out[right];
        out[right] = temp;
        left = increment(left);
        right = decrement(right);
    }

    out[index] = '\0';

    *result = out;

    return Normal;
}
int len(const char* str){
    int size = 0;
    for(; str[size]; size++){
    }
    return size;
}
int main(){
    char *res;
    keyErrs status = convert_to_base_2r(64, 4, &res);
    if (status != Normal){
        switch (status){
        case MallocErr:
            printf("Malloc error occured!\n");
            break;
        case BaseErr:
            printf("Base is 2^r and r must be in natural int from 1 to 5\n");
            break;
        case NumErr:
            printf("Number must be valid natural integer\n");
            break;
        default:
            break;
        }
        return 1;
    }

    printf("%s\n", res);
    free(res);

    return 0;
}