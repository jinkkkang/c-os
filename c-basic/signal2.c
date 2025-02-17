#include <stdio.h>
#include <signal.h>
#include <unistd.h>

/* sigaction 函数是 C 标准库中的一个函数，用于检查或修改与特定信号相关联的处理动作。
与 signal 函数相比，sigaction 提供了更多的控制和更好的移植性。

https://www.runoob.com/cprogramming/c-function-sigaction.html
*/



// 自定义的信号处理函数
void handle_sigint(int sig) {
    printf("Caught signal %d\n", sig);
}

int main() {
    struct sigaction sa;

    // 设置处理程序为 handle_sigint
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // 使用 sigaction 设置新的信号处理动作
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // 主循环，持续运行直到捕捉到信号
    while (1) {
        printf("Running...\n");
        sleep(1);
    }

    return 0;
}