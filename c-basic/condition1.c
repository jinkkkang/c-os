#include <stdio.h>
#include <unistd.h>
#define age 80  


/*
条件表达式的结果如果是非零数值或者非空指针，那么对应的if语句条件被认为是“真”，反之则是“假”。
*/
int main(){ 
    // for (; ;)
    // {
    //     printf("hello \n");
    //     sleep(1);
    // }
    // char name[] = "jinkang";
    // printf("%s\n",name);
    // if (age > 10){
    //     printf("da");
    // }else{
    //     printf("xiao");
    // }
    //如果if下面的表达试只有一行，则可以不用写大括号。
    // if (age < 20)
    //     printf("xiao");
    // else
    //     printf("da");


    // 0 是false, 非0是true
    int a = 0;
    if (a)
        printf("true\n");
    else
        printf("false\n");

    char *name1 = "zhangsan";
    char *name2 = NULL;

    if (name1) {
        printf("name1 is true\n");
    } else {
        printf("name1 is false\n");
    }

    if (name2) {
        printf("name2 is true\n");
    } else {
        printf("name2 is false\n");
    }
    
}