#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#define NETLINK_TEST    30

#define PATHMAX = 100

// struct sig_info_data_test {
// 	int sig;
// 	char send_comm_path;
// 	int  send_pid;

// 	char *send_parent_comm_path;
// 	int send_parent_pid;

// 	char *to_comm_path;
// 	int  to_pid;
// };

struct sig_info_data_test {
	int sig;
	char send_comm_path[PATH_MAX];
	int  send_pid;

	// char *send_parent_comm_path;
	int send_parent_pid;

	char to_comm_path[PATH_MAX];
	int  to_pid;
};



typedef struct _user_msg_info
{
    struct nlmsghdr hdr;
    struct sig_info_data_test msg;
} user_msg_info;

int main(int argc, char **argv)
{
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
        return -1;
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.nl_family = AF_NETLINK; //AF_NETLINK
    saddr.nl_pid = 100;  //端口号(port ID)
    saddr.nl_groups = 0;
    if(bind(skfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
    {
        perror("bind() error\n");
        close(skfd);
        return -1;
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

    // // memcpy(NLMSG_DATA(nlh), umsg, strlen(umsg));
    // ret = sendto(skfd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&daddr, sizeof(struct sockaddr_nl));
    // if(!ret)
    // {
    //     perror("sendto error\n");
    //     close(skfd);
    //     exit(-1);
    // }
    // printf("send kernel:%s\n", umsg);
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
    


    // printf("from kernel netlink info: from: %s %d  to: %s %d\n", u_info.msg.send_comm, u_info.msg.send_pid, u_info.msg.to_comm, u_info.msg.to_pid);
    close(skfd);

    free((void *)nlh);
    return 0;
}