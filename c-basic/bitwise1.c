#include <stdio.h>
  
int main()
{
  int val = 10;
  int c = val << 3;  // 左移3位， 就是 val * 2 ^ 3
  printf("val:%d", c);

  /* 三元运算符实例 */
  int a = 10;
  int  b = (a == 1) ? 20: 30;
  printf( "b 的值是 %d\n", b );
}
 
