#include <stdio.h>

void generate(char basename[], size_t size, int count);

int main(void) {
    generate("foobar", 100 * 1024, 1000);
}

void generate(char basename[], size_t size, int count) {
    char fname[128];
    sprintf(fname, "/mnt/vdb2/%s.txt", basename);

    FILE *myFile = fopen(fname, "w");
    FILE *randomiser = fopen("/dev/urandom", "r");

    int startnum = 0;

    while( startnum < count )
    {
        char bufor[size];
        size_t result = fread(bufor, 1, size, randomiser);
        fwrite(bufor, 1, result, myFile);
        startnum++;
    }

    fclose(myFile);
    fclose(randomiser);

}