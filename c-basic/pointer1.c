#include <stdio.h>
#include <stdlib.h>



/*
void 指针。它只有内存块的地址信息，没有类型信息，等到使用该块内存的时候，再向编译器补充说明，里面的数据类型是什么。
另一方面，void 指针等同于无类型指针，可以指向任意类型的数据，但是不能解读数据。void 指针与其他所有类型指针之间是互相转换关系，
任一类型的指针都可以转为 void 指针，而 void 指针也可以转为任一类型的指针。
*/


int main(void) {
   int x = 10;
//    int * p = &x;
//    printf("x : %d\n",x);
//    printf("*p : %d\n",*p);


//    void* p = &x; // 整数指针转为 void 指针
//    int* q = p; // void 指针转为整数指针
//    printf("*q : %d\n",*q);



      //p是一个 void 指针，所以这时无法用*p取出指针指向的值

     // void* p = &x;
     //  printf("%c", *p); // 编译的时候会报错:  argument type 'void' is incomplete 
}
    
