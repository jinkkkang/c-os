#include <stdio.h>


char name2[128];
int main()
{
    unsigned char name[128]; //局部变量一定要初始化，随机的
    for (size_t i = 0; i < 128; i++)
    {
        if (i % 16 == 0){
            printf("\n");
        }
        printf("%02x ", name[i]);
    }
    printf("\n");
    return 0;
}


//data 段会增加应用程序体积的大小，bss 不会