#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>
 

 /*
   创建一个守护进程的步骤:  double-fork magic
    1. 执行 fork() 函数,  父进程退出，子进程继续
    2. 子进程执行如下三个步骤，以摆脱与环境的关系
     > 修改进程的当前目录为根目录（/) 
     这样做是有原因的，因为daemon一直在运行，如果当前工作路径上包含有根文件系统以外的其他文件系统，那么这些文件系统将无法卸载。因此，常规是将当前工作目录切换成根目录，当然也可以
是其他目录，只要确保该目录所在的文件系统不会被卸载即可。
     > 调用setsid函数。这个函数的目的是切断与控制终端的所有关系，并且创建一个新的会话
     这一步比较关键，因为这一步确保了子进程不再归属于控制终端所关联的会话。因此无论终端是否发送SIGINT、SIGQUIT或SIGTSTP信号，也无论终端是否断开，都与要创建的daemon进程无关，不
会影响到daemon进程的继续执行。
     > 设置文件模式创建掩码为0。
     这一步的目的是让daemon进程创建文件的权限属性与shell脱离关系。因为默认情况下，进程的umask来源于父进程shell的umask。如果不执行umask（0），那么父进程shell的umask就会影响到
daemon进程的umask。如果用户改变了shell的umask，那么也就相当于改变了daemon的umask，就会造成daemon进程每次执行的umask信息可能会不一致。
    3. 再次执行fork,父进程退出，子进程继续
    执行完前面两步之后，可以说已经比较圆满了：新建会话，进程是会话的首进程，也是进程组的首进程。进程ID、进程组ID和会话ID，三者的值相同，进程和终端无关联。那么这里为何还要再执行一
次fork函数呢？
原因是，daemon进程有可能会打开一个终端设备，即daemon进程可能会根据需要，执行类似如下的代码：
int fd = open("/dev/console", O_RDWR);
这个打开的终端设备是否会成为daemon进程的控制终端，取决于两点：
·daemon进程是不是会话的首进程。
·系统实现。（BSD风格的实现不会成为daemon进程的控制终端，但是POSIX标准说这由具体实现来决定）。
既然如此，为了确保万无一失，只有确保daemon进程不是会话的首进程，才能保证打开的终端设备不会自动成为控制终端。因此，不得不执行第二次fork，fork之后，父进程退出，子进程继续。这
时，子进程不再是会话的首进程，也不是进程组的首进程了。
    4. 关闭标准输入（stdin）、标准输出（stdout）和标准错误（stderr）
因为文件描述符0、1和2指向的就是控制终端。daemon进程已经不再与任意控制终端相关联，因此这三者都没有意义。一般来讲，关闭了之后，会打开/dev/null，并执行dup2函数，将0、1和2重定
向到/dev/null。这个重定向是有意义的，防止了后面的程序在文件描述符0、1和2上执行I/O库函数而导致报错。
 */



int task(void){
    syslog(LOG_INFO, "Daemon started");
    while (1) {
        syslog(LOG_INFO, "Daemon is running...");
        sleep(3);
    }

    closelog();
    return 0;
}
int main(void) {
    int status;

    // 调用daemon()函数，参数0表示改变工作目录，1表示保留文件描述符
    status = daemon(0, 1);

    if (status == -1) {
        // 如果daemon()失败，打印错误信息
        perror("Failed to daemonize");
        return 1;
    }

    task();
    return 0

}