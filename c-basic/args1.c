#include <stdio.h>

int main( int argc, char *argv[] )  
{
   for (size_t i = 0; i < argc; i++)
   {
     printf("%zu : %s\n", i, argv[i]);
   }
   
   if( argc == 2 )
   {
      printf("The argument supplied is %s\n", argv[1]);
   }
   else if( argc > 2 )
   {
      printf("Too many arguments supplied.\n");
   }
   else
   {
      printf("One argument expected.\n");
   }
}



