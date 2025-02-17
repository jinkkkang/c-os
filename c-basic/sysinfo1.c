#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

//https://blog.csdn.net/weixin_46158019/article/details/133327950

int main(int argc,char **argv)
{
     int count1 = get_nprocs_conf();
     int count2 = get_nprocs();
     printf("cpu count1:%d,count2:%d\n",count1,count2);

}