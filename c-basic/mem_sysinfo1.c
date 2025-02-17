#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

//https://blog.csdn.net/weixin_46158019/article/details/133327950

int main(int argc,char **argv)
{
    /*2. 获取当前系统内存使用情况*/
    struct sysinfo s_info;
    char info_buff[100];
    if(sysinfo(&s_info)==0) {
        sprintf(info_buff,"总内存: %.ld M",s_info.totalram/1024/1024);

        printf("%s\n",info_buff);

        sprintf(info_buff,"未使用内存: %.ld M",s_info.freeram/1024/1024);
        printf("%s\n",info_buff);
    }

}