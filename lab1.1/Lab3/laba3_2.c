#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdarg.h"

typedef enum Err{
    Normal,
    Diff_Dimension
}Err;

typedef struct Vector{
    unsigned long long n;
    double* coor;
}Vector;

Vector create_vector(unsigned long long n, ...){
    va_list args;
    Vector vector = {0, NULL};
    vector.n = n;
    va_start(args, n);
    for(int i = 0; i < n; i++){
        vector.coor[i] = va_arg(args, double);
    }
}
double max(unsigned long long n, Vector arr[n], Vector* max){
    int count = 0;
    int mx = 0;
    int* longest_vect_buff;
    for(int i = 0; i < n; i++){
        double len = 0;
        if(n != arr[i].n){
            return Diff_Dimension;
        }
        for(int j = 0; arr[i].coor[j]; j++){
            len += (arr[i].coor[j] * arr[i].coor[j]);
        }
        len = sqrt(len);
        if (mx < len){
            mx = len;
            longest_vect_buff = arr[i].coor;
        }
    }
    max->coor = longest_vect_buff;
    max->n = n;

}