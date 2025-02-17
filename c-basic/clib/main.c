#include <stdio.h>
#include "mylib1.h"
#include "mylib2.h"


int main(int argc, char* argv[]) {
  int ret = sum(2, 5);
  printf("main call mylib1 function sum: %d\n", ret);
  int ret2 = sub(5, 2);
  printf("main call mylib2 function sub: %d\n", ret2);
  return 0;
}