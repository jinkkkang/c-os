#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
 #include <sys/stat.h>
int main()
{
        pid_t pid;
        pid = getpid();
        printf("pid:%d\n",pid);
        sleep(2);
        return 0;

}