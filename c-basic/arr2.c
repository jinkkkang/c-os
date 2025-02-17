#include <stdio.h>

int main() {
    char klist[5] = { 0 };

    printf("klist length: %d\n", sizeof(klist)/sizeof(char));

    for (size_t i = 0; i < sizeof(klist)/sizeof(char); i++)
    {
        printf("klist index: %d, value: %c\n", i, klist[i]);
    }
}