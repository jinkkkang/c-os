#include <stdio.h>
#include <errno.h>
#include <string.h>
 

/*
C 语言提供了 perror() 和 strerror() 函数来显示与 errno 相关的文本消息。

    perror() 函数显示您传给它的字符串，后跟一个冒号、一个空格和当前 errno 值的文本表示形式。
    strerror() 函数，返回一个指针，指针指向当前 errno 值的文本表示形式。
*/

int main ()
{
   
   FILE * pf = fopen ("afilenotexist.txt", "rb"); //文件打开失败后，返回的将是NULL，成功的话，返回的是文件描述符指针
   if (pf == NULL)
   {
      fprintf(stderr, "错误号: %d\n", errno);
      perror("通过 perror 输出错误");
      fprintf(stderr, "打开文件错误: %s\n", strerror( errno ));
   }
   else
   {
      fclose (pf);
   }
   return 0;
}

// 输出效果如下：
// 错误号: 2
// 通过 perror 输出错误: No such file or directory
// 打开文件错误: No such file or directory