#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
 
int main()
{
        pid_t pid = getpid();
        printf("pidA:%d\n",pid);
        pid = fork();
        if (pid > 0) {
            printf("father return: %d\n", getpid());
        } else if ( pid ==0 ) {
            printf("fork return: %d\n", getpid());
        }
        return 0;
}




// fork函数被原/父进程调用一次，但是却返回两次；一次是返回到父/原进程，一次是返回到新创建的子进程。
// fork()返回值	结果
// <0	获取进程失败
// >0	返回父进程或调用者。该值包含新创建的子进程的进程ID
// ==0	返回到新创建的子进程。