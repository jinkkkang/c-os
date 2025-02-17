#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUMBER_OF_THREADS 10
 

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
int main(void){
    pthread_t threads[NUMBER_OF_THREADS];
    int status = 0;
	int i = 0;
    for(i=0; i < NUMBER_OF_THREADS; i++){
    	printf("Main here. Creating thread %d\n",i);
        //创建线程，线程函数传入参数为i
    	status=pthread_create(&threads[i], NULL, printf_hello_world, &i);
		if(status != 0){//线程创建不成功，打印错误信息
    		printf("pthread_create returned error code %d\n", status);
    		exit(-1);
		}
	}
    int results[NUMBER_OF_THREADS];
    void *thr_ret;
	for(i=0; i < NUMBER_OF_THREADS; i++){
		pthread_join(threads[i], &thr_ret);
        results[i] = *(int*)thr_ret;
	}

    for(i=0; i < NUMBER_OF_THREADS; i++){
		printf("Thread %d returned %d\n", i, results[i]);
	}
	exit(0);
}
void* printf_hello_world(void* tid){
	printf("Hello world %d.\n", *(int*)tid);//在线程函数中打印函数的参数
    int *ret = (int*)malloc(sizeof(int));
    *ret = 0;
    if(*(int*)tid > 4){
    	*ret = 1;
	}
    return ret;
	//pthread_exit(ret);
}


void* print_hello_world_2(void* tid){
	int val = *(int*)tid;
	int* a=(int*)malloc(sizeof(int));
	*a= val * val;
	return a;
}