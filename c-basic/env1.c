#include <stdio.h>
#include <stdlib.h>


/*
char * getenv(const char *name);
getenv (取得环境变量内容)
函数说明 getenv()用来取得参数name 环境变量的内容. 参数name 为环境变量的名称, 如果该变量存在则会返
回指向该内容的指针. 环境变量的格式为name＝value.
返回值执行成功则返回指向该内容的指针, 找不到符合的环境变量名称则返回NULL.


putenv (改变或增加环境变量)
相关函数 getenv, setenv, unsetenv
头文件 #include4<stdlib.h>
定义函数 int putenv(const char * string);
函数说明 putenv()用来改变或增加环境变量的内容. 参数string 的格式为name＝value, 如果该环境变量原
先存在, 则变量内容会依参数string 改变, 否则此参数内容会成为新的环境变量.


*/

extern char **environ; // environ 是一个外部全局变量，需要声明

void print_environment_variables() {
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

int main() {
    // char *result = getenv("USER2");
    char *result;
    if ((result = getenv("USER2"))) {
        printf("user :%s\n", result);
    }else {
        printf("no env\n");
    }

    if (setenv("kkk","ooooo",1) != -1 ) {
        printf("set env ok \n");
    }

    print_environment_variables();

    char *resultnew;
    if ((resultnew = getenv("kkk"))) {
        printf("kkk :%s", resultnew);
    }else {
        printf("no env");
    }

    unsetenv("kkk");
    system("export uuu=ppp");
    


}
