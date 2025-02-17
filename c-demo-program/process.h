#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>





/*
#include <pthread.h>//需要添加pthread.h头文件
int pthread_create(
                 pthread_t *thread,   //指向线程标识符的指针，用pthread_t创建
                 const pthread_attr_t *attr,  //设置线程属性，默认为NULL
                 void *(*start_rtn)(void *), //线程运行函数的起始地址
                 void *arg //传递给线程函数的参数
                  );

int pthread_join(pthread_t thread,    //线程标识符，即线程ID，标识唯一线程
                 void **retval);    //用户定义的指针，用来存储被等待线程的返回值。
//返回值：0：成功；其他：失败的错误号

gcc thread.c   -lpthread
*/
void* printf_hello_world(void* tid);

void* ps_monitor_start(void* tid);



void ps_monitor_init(void);