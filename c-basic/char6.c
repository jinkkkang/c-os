#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  
  


int main() {  
    struct student {  
        char *name;
        int age;
    };

    // struct student2 {  
    //     char name[10];  
    //     int age;
    // };

    struct student s1;
    s1.name = (char *)malloc(1024 * sizeof(char));
    strcpy(s1.name, "zhangsan");
    printf("s1: %s\n", s1.name);
    printf("s1 sizeof: %zu\n", sizeof(s1.name));
  

    // struct student2 s2;
    // memset(s2.name, 0, sizeof(s2.name));
    // strcpy(s2.name, "lisi");
    // free(s1.name);
    // printf("s2: %s\n", s2.name);
    // free(s1.name);
    // return 0;  
}