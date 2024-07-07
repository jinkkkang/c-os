#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int* p = malloc(sizeof(int)); //malloc()  分配内存失败返回的将是NULL
    *p = 12;
    printf("%d\n", *p); // 12
    free(p); //free()用于释放malloc()函数分配的内存
    return 0;
}


/*
calloc()函数的作用与malloc()相似，也是分配内存块。该函数的原型定义在头文件stdlib.h。

两者的区别主要有两点：

（1）calloc()接受两个参数，第一个参数是某种数据类型的值的数量，第二个是该数据类型的单位字节长度。
 (2) calloc()会将所分配的内存全部初始化为0。malloc()不会对内存进行初始化，如果想要初始化为0，还要额外调用memset()函数。
*/
int main(void) {    
    int* p = calloc(10, sizeof(int));
    *p = 20;
    printf("%d\n", *p);  //20
    // 等同于
    int* p = malloc(sizeof(int) * 10);
    memset(p, 0, sizeof(int) * 10); 
    printf("%d\n", *p);  // 0

}