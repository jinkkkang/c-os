#include <stdio.h>
 
int main ()
{
   int value = 10;
   int *address = &value;
   printf("value add = %x\n", address);
   printf("valnew = %d\n", value);
   for (size_t i = 0; i < 100; i++)
   {
      /* code */
   }
   
   int *ptr = (int *)address;
   printf("valnew22 = %d\n", *ptr);
}