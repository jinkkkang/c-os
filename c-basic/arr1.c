#include <stdio.h>

int main() {
    int klist[5];
    for (int i = 0; i < 5; i++)
    {
        klist[i] = i + 10;
    }
    // printf("klsit: %d\n", klist);
    int *first;
    first = klist;
    int* ptr2 = &klist[0];
    printf("klist first address: %p\n", ptr2);
    printf("klist first address: %p\n", klist);

    for (size_t i = 0; i < 10; i++)
    {
        printf("klist address: %p\n", first);
        printf("klist value: %d\n", *first);
        first++;
        // printf("klsit: %d\n", *first);
    }
    
    
}