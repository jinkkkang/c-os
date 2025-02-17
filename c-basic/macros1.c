#include <stdio.h>

// #define DEBUG
int main(void)
{
    #ifdef DEBUG
    printf("DEBUG\n");
    #else
    printf("NOT DEBUG\n");
    #endif
}


// #ifndef  如果未定义