#include <stdio.h>
#include <string.h>

#include <stdio.h>  
  
int main() {  
    struct student {  
        char *name;  
        int age;
    };

    struct student2 {  
        char name[10];  
        int age;
    };


    struct student s1 = {"zhangsan1", 18};
    struct student2 s2 = {"zhangsan2", 18};

    printf("sizeof :%lu\n", sizeof(struct student));
    printf("sizeof :%lu\n", sizeof(s1));
    printf("name: %s\n", s1.name);
  

    printf("sizeof :%lu\n", sizeof(struct student2));
    printf("sizeof :%lu\n", sizeof(s2));
    printf("name: %s\n", s2.name);
    return 0;  
}