#include <stdio.h>
#include <string.h>
 
int main ()
{
   char str1[14] = "runoob";
   char str2[14] = "google";
   char str3[14];
   char kangkang[] = "kangkang";
   char kangwen[] = "kangwen";
   char kangkang2[] = "kangkang";
   int  len ;
 
   /* 复制 str1 到 str3 */
   strcpy(str3, str1);
   printf("strcpy( str3, str1) :  %s\n", str3 );
 
   /* 连接 str1 和 str2 */
   strcat( str1, str2);
   printf("strcat( str1, str2):   %s\n", str1 );
   

   //strncat，  将 src 字符串的前 n 个字符追加到 dest 字符串的末尾
   char dest[20] = "Hello, ";  
   char src[] = "Beautiful World!";  
   strncat(dest, src, 5); // 只追加前5个字符  
   printf("%s\n", dest); // 输出: Hello, Beaut  
    return 0;  
   /* 连接 str1 和 s
   /* str1 的长度 */
   len = strlen(str1);
   printf("strlen(str1) :  %d\n", len );

   //strcmp: 如果 s1 和 s2 是相同的，则返回 0；如果 s1<s2 则返回小于 0；如果 s1>s2 则返回大于 0。
   printf("kangkang strcmp kangkang2: %d\n", strcmp(kangkang,kangkang2));
   printf("kangkang strcmp kangwen: %d\n", strcmp(kangkang,kangwen));

   //strstr(s1, s2); 在字符串中查找子串 
   // 返回一个指针，指向字符串 s1 中字符串 s2 的第一次出现的位置。
   char* originstr = "hellllo world kwwerwr";
   char* result = strstr(originstr, "world");
   printf("result:%s\n", result);
   if (result){
      printf("find it");
   }else{
      printf("not find it");
   }


   return 0;
}