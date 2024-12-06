#include "raylib.h"
#include <stdio.h>

int main()
{
    int index;
    int* iptr = &index;
    char arr[10];

    for (*iptr = 0; *iptr < 10; (*iptr)++) {
        arr[*iptr] = *iptr * *iptr;
        printf("%d: %d\n", *iptr, arr[*iptr]);
    }
    
    return 0;
}