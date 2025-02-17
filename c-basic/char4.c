#include <stdio.h>
#include <string.h>


/*
   总是推荐使用snprintf， 而不是 sprintf
   int snprintf(char *str, size_t size, const char *format, ...);
   str：指向用于存储结果字符串的缓冲区的指针。
   size：缓冲区的大小，包括结尾的空字符（\0）。
   如果生成的字符串（不包括结尾的空字符）的长度大于或等于 size，则函数不会在 str 中写入任何东西，
   除了可能的情况是在 str[0] 中写入一个空字符（这取决于具体实现）。

*/
int main ()
{
   char site[5];

   /*
   像第一个例子，栈溢出，会有相关的告警
   char4.c:17:4: warning: 'sprintf' will always overflow; destination buffer has size 5, but format string expands to at least 9 [-Wfortify-source]
   */

   // sprintf(site,"www.%s.com", "google");
   // printf("site = %s, length = %lu\n", site, strlen(site));


   snprintf(site, sizeof(site), "www.%s.com", "google");
   printf("site = %s, length = %lu\n", site, strlen(site));

   //输出结果:  site = www., length = 4 (结尾空字符\n占一个)

}