#include <stdio.h>

int testhello(int a){
    int ret = a + 3;
    return ret;
}
int main() {
    int res = testhello(10);
    return res;
}