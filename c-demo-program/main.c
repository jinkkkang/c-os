#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h> 

// #include <sys/prctl.h>
// #include <sys/stat.h>
// #include <sys/types.h>

#include "main.h"
#include "process.h"

int log_fd = -1;

typedef enum monitor_type {
    PS_ITEM,
    FS_ITEM,
    FILE_ITEM,
    DISK_ITEM,
    INODE_ITEM,
    CUSTOM_DAEMON_ITEM,
    CUSTOM_PERIODIC_ITEM,
    IO_DELAY_ITEM,
    SYSTEM_ITEM,
    SYS_EVENT_ITEM, 
    ZOMBIE_ITEM, // 可以通过从 内核那边拿，遍历所有的 task_struct 拿
    MONITOR_ITEMS_CNT
} monitor_item_type;



int init_log()
{
    log_fd = open(LOG_FILE_PATH, O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC, LOG_FILE_PERMISSION);
    if (log_fd < 0) {
        (void)printf("[sysmonitor] open '%s' failed, process will exit.errno[%d]\n", LOG_FILE_PATH, errno);
        return 1;
    }   
    return 0;
}

int close_log()
{
    if (log_fd > 0) { 
        close(log_fd);
    }
}

int write_log(const char *msg)
{
    int write_ret = -1;
    write_ret = write(log_fd, msg, strlen(msg));
    if (write_ret == -1) {
        (void)printf("[wenaodemo]: write to log file failed, errno[%d]. errmsg: %s\n", errno, strerror(errno));
    }
}
int main(int argc, char const *argv[])
{
    // if (argc == 1){
    //     printf("usage error");
    //     return 1;
    // }
    int log_ret;
    log_ret = init_log();
    if (log_ret != 0) {
        goto err;
    }
    char s[] = "hello world ss1";
    write_log(s);
    ps_monitor_init();
    err:
        close_log();
        exit(EXIT_FAILURE);
        printf("error");


}