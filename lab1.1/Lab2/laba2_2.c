#include "math.h"
#include "stdio.h"
#include "stdarg.h"
#include "float.h"
typedef enum Err{
    NEGATIVE_SQRT = 2,
    OVERFLOWED,
}Err;

double midGeom(int count, ...){
    double ans = 1;
    va_list args;
    va_start(args, count);
    for(int i = 0; i < count; i++){
        double x = va_arg(args, double);
        if (x < 0) return NEGATIVE_SQRT;
        if (ans > DBL_MAX / x) return OVERFLOWED;
        ans *= x;
    }
    va_end(args);
    ans = pow(ans, 1.0 / count);
    return ans;
}
double my_pow(double to_pow, int power){
    if (power == 0) return 1;
    if (power < 0) return my_pow(1 / to_pow, -power);
    if (power % 2 == 0){
        double ans = my_pow(to_pow * to_pow, power / 2);
        if (!isfinite(ans)){
            return OVERFLOWED;
        }
        return ans;
    }
    else {
        double ans = to_pow * my_pow(to_pow * to_pow, (power - 1) / 2);
        if (!isfinite(ans)){
            return OVERFLOWED;
        }
        return ans;
    }

}
int main(int argc, char** argv){
    double res = midGeom(4, 4.5, 5.6, 7.0, 8.987);
    if (res == NEGATIVE_SQRT) {
        printf("Negative sqrt");
        return 1;
    }
    if (res == OVERFLOWED){
        printf("Overflowed");
        return 1;
    }
    printf("mid geom: %lf\n", res);
    double res2 = my_pow(2, 210);
    if (res2 == OVERFLOWED){
        printf("Overflowed");
        return 1;
    }
    printf("power: %lf\n", res2);

    return 0;
}