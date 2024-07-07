// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>


/*
fgets(char * s, int size, FILE * stream);
函数说明 fgets()用来从参数stream 所指的文件内读入字符并存到参数s 所指的内存空间, 直到出现换行字符、
读到文件尾或是已读了size-1 个字符为止, 最后会加上NULL 作为字符串结束.
返回值 gets()若成功则返回s 指针, 返回NULL 则表示有错误发生.

*/

// void test_popen(const char *name)
// {   
//     FILE *fp;
//     char buffer[64] = {'\0'};
    
//     fp = popen(name, "r");
//     if(NULL == fp)
//     {
//         printf("popen failed!!!\n");
//         return;
//     }

//     while(fgets(buffer, sizeof(buffer), fp))
//     {
//         printf("test_popen get:%s\n", buffer);
//     }
    
//     pclose(fp);
    
// }

// int main()
// {
//     time_t now = time(NULL);
//     printf("enter:main -- %s\n", ctime(&now));

//     test_popen("./syscall");

//     now = time(NULL);
//     printf("leave:main -- %s\n",ctime(&now));    

//     return 0;
// }

#include <stdlib.h>  
#include <stdio.h>  
  
#define BUF_SIZE 10
char buf[BUF_SIZE];  
  
int main(void)  
{  
    FILE * p_file = NULL;  
   
    p_file = popen("ifconfig en4 && exit 3", "r");  
    if (!p_file) {  
        fprintf(stderr, "Erro to popen");  
    }  
  
    while (fgets(buf, BUF_SIZE, p_file) != NULL) {  
        fprintf(stdout, "out11: %s", buf);  
    }  
   // 关闭管道并获取退出状态
   int result;
   /*
    在这个例子中，WIFEXITED()宏用于检查进程是否正常退出（即没有接收到信号而结束）。如果进程正常退出，
    WEXITSTATUS()宏则用于提取退出状态码。如果进程是由于接收到信号而终止的，则使用WIFSIGNALED()和WTERMSIG()
    进行相应的判断。
    */
   result = pclose(p_file);

    // 检查退出状态
    if (result == -1) {
        perror("pclose failed");
    } else {
        // 使用waitpid返回值格式化的宏判断退出原因
        if (WIFEXITED(result)) {
            int exit_status = WEXITSTATUS(result);
            printf("Command exited normally with status code: %d\n", exit_status);
            // 如果exit_status为0，通常表示命令执行成功
            if (exit_status == 0) {
                printf("Command executed successfully.\n");
            } else {
                printf("Command executed with errors (status code %d).\n", exit_status);
            }
        } else if (WIFSIGNALED(result)) {
            int term_signal = WTERMSIG(result);
            printf("Command was terminated by signal: %d\n", term_signal);
        } else if (WCOREDUMP(result)) {
            printf("Core file was produced.\n");
        }  // 其他可能的状态可以根据需要添加检查
    }

    return 0;


   // char zhangsan[100];
   // sprintf(zhangsan,"hello %s world", "zhangsan");
   // printf("res: %s\n",zhangsan);
}  
