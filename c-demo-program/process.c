#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <pthread.h>
#include "process.h"

#define NETLINK_TEST    30

#define PATHMAX = 100

void* signal_monitor2(void* tid){
    int skfd;
    int ret;
    user_msg_info u_info;
    socklen_t len;
    struct nlmsghdr *nlh = NULL;
    struct sockaddr_nl saddr, daddr;

    /* 创建NETLINK socket */
    //int socket(int domain, int type, int protocol);
    skfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
    if(skfd == -1)
    {
        perror("create socket error\n");
        return;
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.nl_family = AF_NETLINK; //AF_NETLINK
    saddr.nl_pid = 100;  //端口号(port ID)
    saddr.nl_groups = 0;
    if(bind(skfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    {
        perror("bind() error\n");
        close(skfd);
        return;
    }

    memset(&daddr, 0, sizeof(daddr));
    daddr.nl_family = AF_NETLINK;
    daddr.nl_pid = 0; // to kernel
    daddr.nl_groups = 0;

    int len_of_info;
    len_of_info = sizeof(struct sig_info_data_test);
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(len_of_info));
    memset(nlh, 0, sizeof(struct nlmsghdr));
    nlh->nlmsg_len = NLMSG_SPACE(len_of_info);
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_type = 0;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = saddr.nl_pid; //self port

    while (1)
    {
        memset(&u_info, 0, sizeof(u_info));
        len = sizeof(struct sockaddr_nl);
        ret = recvfrom(skfd, &u_info, sizeof(user_msg_info), 0, (struct sockaddr *)&daddr, &len);
        if(!ret){
            perror("recv form kernel error\n");
            close(skfd);
            exit(-1);
        }
        printf("from kernel netlink info: sig: %d, from %s  sendpid: %d sendto: %s topid: %d\n", u_info.msg.sig, u_info.msg.send_comm_path,u_info.msg.send_pid, u_info.msg.to_comm_path,u_info.msg.to_pid);
        sleep(1);
    }
    close(skfd);

    free((void *)nlh);
    return;
}


void* signal_monitor(void* tid){
    while (1)
    {
       printf("signal_monitor2\n");
       sleep(1);
    }
    
}
int signal_monitor_init(void)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, signal_monitor, NULL)) {
        printf("signal_monitor pthread_create error %d\n", errno);
        return;
    }
    // pthread_setname_np(tid, "kangtest"); # not work
    pthread_join(tid, NULL);
}



