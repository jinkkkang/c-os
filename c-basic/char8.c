#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>  
  
char * test(){
    char *str1 = "hello world wenao"; // 指向的是常量区（只读数据段）的一个固定地址
    char str2[] = "hello world wastack"; // 保存在栈里.
    //static 后的变量不会放在栈中了，而是放在 data 段中.
    static char str4; //未初始化的静态变量，保存在data段的 static区中
    static char str5[] = "hello world 3"; //初始化的静态变量，保存在data段的 static区中
    return str5;
}


int main() {
    char *name = test();
    printf("88:%s\n", name);
}

// objdump -s  ./a.out  | less
// objdump -s -j .rodata hello

// readelf -p .interp  ./a.out