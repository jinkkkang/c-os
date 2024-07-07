#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


int main(int argc, const char *argv[])
{
   
        FILE * fp;
        fp = fopen("/disktest/test.txt", "a+");
        if (fp == NULL) {
            printf("Error opening file\n");
            printf("error_code: %d\n", errno);
            printf("Error opening file: %s\n", strerror(errno)); //strerror from string.h
            return 1;
        }
        int count = 10;
        for (size_t i = 0; i < count; i++)
        {
            fputs("helloworld\n", fp);
            fprintf(fp, "%s : %zu \n", "helloworld", i);
            sleep(1);
        }

        fclose(fp);
}
