#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/*
FILE *fopen(const char *filename, const char *mode);
 mode:  如 "r"、"w"、"a"
  

还提供一些 fread, fwrite, fprintf, fscanf 等标准库函数来进行文件的读写和格式化操作。
*/
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
