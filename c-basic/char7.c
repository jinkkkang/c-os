#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  
  
int main() {  

    char *name2;
    printf("name2 sizeof: %zu\n", sizeof(name2)); //输出8字节，64位系统上指针大小是8字节
    name2 = (char *)malloc(20 * sizeof(char));
    printf("name2 point  sizeof: %zu\n", sizeof(name2));
    printf("name2 content sizeof: %zu\n", sizeof(*name2));
    printf("name2 content strlen: %zu\n", strlen(name2));
    strcpy(name2, "hello world");
    printf("name2 point  sizeof: %zu\n", sizeof(name2));
    printf("name2 content sizeof: %zu\n", sizeof(*name2));
    printf("name2 content strlen: %zu\n", strlen(name2));
    printf("name2: %s\n", name2);
    char name3[10];
    printf("name3 sizeof: %zu\n", sizeof(name3));
    printf("name3 strlen: %zu\n", strlen(name3));

}