#include <stdio.h>  
#include <stdlib.h>  
#include <dirent.h>  
#include <string.h>  
  
static int select(const struct dirent *entry) {  
    // 可以在这里添加过滤逻辑，例如只选择文件名以"."开头的目录项  
    // 这里我们不过滤任何内容  
    return 1;  
}  
  
int main(int argc, char *argv[]) {  
    struct dirent **namelist;  
    int n;  
  
    if (argc != 2) {  
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);  
        exit(EXIT_FAILURE);  
    }  
  
    n = scandir(argv[1], &namelist, select, alphasort);  
    if (n < 0) {  
        perror("scandir");  
        exit(EXIT_FAILURE);  
    }  
  
    while (n--) {  
        printf("%s\n", namelist[n]->d_name);  
        free(namelist[n]);  
    }  
    free(namelist);  
  
    exit(EXIT_SUCCESS);  
}