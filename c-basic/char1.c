#include <stdio.h>
 
int main ()
{
   char site[] = "helloworld";
 
   printf("菜鸟教程: %s\n", site ); 
   size_t allsize = sizeof(site);
   size_t num = allsize / sizeof(char);
   for (size_t i = 0; i < num; i++)
   {
      printf("%c %zu \n",*(site+i), i);
      if (*(site+i) == "\\0") {
         printf("end\n");
      }
   }
   
   return 0;
}