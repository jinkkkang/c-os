#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>



#include "process.h"

void* sigal_monitor_init(void* tid){
    (void)prctl(PR_SET_NAME, "monitor-process");
    for (;;) {
        printf("ps_monitor_start %d\n", tid);
        sleep(3);
    }
    return 0;
	//pthread_exit(ret);
}

void sigal_monitor_init(void)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, sigal_monitor_init, NULL)) {
        printf("pthread_create error %d\n", errno);
        return;
    }
    // pthread_setname_np(tid, "kangtest"); # not work
    pthread_join(tid, NULL);
}