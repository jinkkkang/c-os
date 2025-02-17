#include <stdio.h>
  
#define flaga (1<<0) //#01
#define flagb (1<<1) //#10
#define flagc (1<<2) //#100
int main()
{
  printf("flaga:%d \n",  flaga); //1
  printf("flagb:%d \n", flagb);  //2
  printf("flagc:%d \n", flagc);  //4
  unsigned flag = 0;
  flag |= flaga; // 设置flaga 
  printf("flag: %d \n",  flag); //1
  flag |= flagb; // 设置flagb
  printf("flag: %d \n",  flag); //3
  flag |= flagc; // 设置flagc 
  printf("flag: %d \n",  flag); //7

  flag &= ~flagb; // 消除flagb
  printf("flag: %d \n",  flag);

  if (flag & flaga) {
    printf("flaga is set \n");
  } else {
    printf("flaga is not set \n");
  }

  if (flag & flagb) {
    printf("flagb is set \n");
  } else {
    printf("flagb is not set \n");
  }

  if (flag & flagc) {
    printf("flagc is set \n");
  } else {
    printf("flagc is not set \n");
  }
}
 
