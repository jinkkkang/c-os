#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

/*
c语言判断文件是否存在
方法1，用access 方法，如果检查成功，则返回0；否则返回-1，并将errno设置为相应的错误码。可以使用perror()函数输出具体的错误信息。
参考 https://juejin.cn/s/c%E8%AF%AD%E8%A8%80%20access%E5%87%BD%E6%95%B0


access 函数参数 int access(const char *pathname, int mode);

pathname: 文件路径
mode: 
R_OK：检查读权限
W_OK：检查写权限
X_OK：检查执行权限
F_OK：检查文件是否存在

方法2, fopen，判断返回是否为NULL, NULL则打开失败

方法3, 我们可以使用stat函数来判断文件是否存在。stat函数在sys/stat.h头文件中声明，它会将文件的状态信息填充到struct stat类型的结构体中。
如果文件不存在，stat函数会返回-1，并设置errno为ENOENT

*/
int exists(const char *path, int type) {
    return access(path, type) == 0;
}

// 检查是否可读
int is_readable(const char *filename) {
    return exists(filename, R_OK);
}

// 检查是否可写
int is_writable(const char *filename) {
   return exists(filename, W_OK);
}

// 检查是否可执行
int is_executable(const char *filename) {
    return exists(filename, X_OK);
}




int main()
{
    char *filename = "example.txt";
    // if (access(filename, F_OK) == 0) {
    //     printf("%s exists\n", filename);
    // } else {
    //     printf("error_code: %d\n", errno);
    //     printf("Error opening file: %s\n", strerror(errno)); //strerror from string.h
    //     return 1;
    // }
    // int res = is_readable(filename);
    // printf("%d\n",res);
    // return 0;

    FILE *filetest = NULL;
    filetest = fopen(filename, "r");
    if (filetest == NULL) {
        printf("file open filed");
    }else{
        printf("file open ok");
    }
    fclose(filetest);

    struct stat buffer;
    // 检查文件是否存在
    if (stat("myfile.txt", &buffer) == 0) {
        printf("File exists\n");
    } else {
        printf("File does not exist\n");
    }
    return 0;
}


 