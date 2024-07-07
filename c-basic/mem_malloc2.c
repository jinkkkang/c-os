#include <stdlib.h>
int main () {
        int i;
        // int *p=malloc(1024*1024*10);
        void *p = malloc(1024*1024*10);
        print("address: %s\n", p);
        p = (*int)p;
        for (i=0; i<1000; i++) {
                p[i]=i;
        }
        while (1) {
                sleep(100);
        }
        return 0;
}